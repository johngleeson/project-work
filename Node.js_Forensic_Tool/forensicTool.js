/*
    forensicTool.js
    Created by John Gleeson 12162213 on 10/02/16
    Uses Node.js

    Run program by /path/to/node forensicTools.js <imagename>
    If Node is in your environmental variables you can run it by: node forensicTools.js <imagename>
 */

var fs = require('fs');
var convert = require('binstring');
var switches = require('./switches.js');
var little = require('./littleEnd.js');
var clusterTwoLocation;
var clusterSectorAddress;
var noSectorsPerCluster;

//process.argv[2] is the filename to be processed
fs.open(process.argv[2], 'r', function (status, fd) {
    if (status) {
        console.log("\nUnable to open that file. Does it exist?");
        console.log(status.message+"\n");
        return;
    }
    //calls the readFirstSector function and passes the file descriptor as the argument
    console.log("\n\n\n");
    readFirstSector(fd);

});

////////////////////////////////////////////////////////////////////////////////////////
//
//     READING FUNCTIONS
//
///////////////////////////////////////////////////////////////////////////////////////
function readFirstSector (fd) {

    //Creates a new buffer to hold one cluster
    var partitionBuffer = new Buffer(512);
    fs.read(fd, partitionBuffer, 0, 512, 0, function (err) {

        if (err) {
            console.log("Unable to read first sector of disk image.\n" + err);
        } else {

            //Calls the extractPartitionTable function and stores the return value as startSectorsAsArray
            var startSectorsAsArray = extractPartitionTable(partitionBuffer);

            //As we're only interested in the first start sector, stores that as a variable
            var bootSectorOfFirstPartition = startSectorsAsArray[0];

            //Calls the readBootSector function with the file descriptor (of the disk image) and boot sectors as arguments
            readBootSector(fd,bootSectorOfFirstPartition);
        }

    });
}

function readBootSector (fd,sectorNumber) {
    var partitionBuffer = new Buffer(512);

    // Uses the sector that's passed to it as an offset to read just the cluster we're interested in
    fs.read(fd, partitionBuffer, 0, 512, 512 * sectorNumber, function (err) {

        if (err) {
            console.log("Unable to read boot sector of partition.\n" + err);
        } else {

            var rootDirSector = extractBootSector(partitionBuffer,sectorNumber);

            readRootDir(fd,rootDirSector);

        }

    });
}

function readRootDir (fd,sectorNumber) {
    var partitionBuffer = new Buffer(512);

    // Uses the sector that's passed to it as an offset to read just the cluster we're interested in
    fs.read(fd, partitionBuffer, 0, 512, 512 * sectorNumber, function (err) {

        if (err) {
            console.log("No root directory found.\n" + err);
        } else {

            extractRootDir(partitionBuffer,sectorNumber);

            if(clusterSectorAddress != 0){
                readDeletedFile(fd,clusterSectorAddress);
            } else {
                console.log("Deleted file not found.");
            }
        }

    });
}

function readDeletedFile (fd,sectorNumber) {
    var partitionBuffer = new Buffer(512);

    // Uses the sector that's passed to it as an offset to read just the cluster we're interested in
    fs.read(fd, partitionBuffer, 0, 512, 512 * sectorNumber, function (err) {

        if (err) {
            console.log("No deleted file found.\n" + err);
        } else {

            extractDeletedInfo(partitionBuffer);
        }

    });
}



////////////////////////////////////////////////////////////////////////////////////////
//
//     EXTRACTING FUNCTIONS
//
///////////////////////////////////////////////////////////////////////////////////////

// Returns the start sectors of all partitions
function extractPartitionTable(buffer) {

    // convert the buffer (512 bytes) to hex
    var hex = convert(buffer, {in: 'binary', out: 'hex'});

    // convert hex to an array (mainly so I can visualise it)
    var hexArray = hex.match(/.{1,32}/g);
    //console.log(hexArray);

    // array to keep the partition info in
    var partitions = [];
    var startSectors = [];

    // vars for the offset, hexArray[27] and hexArray[28] is where the partition info begins
    var i, j = 27, k = 28;

    for (i = 0; i <= 3; i++) {

        // issue: forgot that it was for more than one partition. Stuck this in a loop to compensate
        partitions[i] = hexArray[j] + hexArray[k];
        j++;
        k++;

        // For each partition, isolates a substring that we're interested in.
        // Not pretty but it works.
        var partitionTableEntry = partitions[i].substring(28, 60);
        console.log("Partition Entry " + [i + 1] + " :" + partitionTableEntry);

        var bootableFlag = partitionTableEntry.substring(0, 2);
        console.log("Flag: 0x" + bootableFlag +" "+ switches.isPartitionActive(bootableFlag));

        var typeOfPartition = partitionTableEntry.substring(8, 10);
        console.log("Type: 0x" + typeOfPartition +""+ switches.partitionType(typeOfPartition));

        var startLBAAddress = partitionTableEntry.substring(16, 24);
        startSectors[i] = parseInt(little.littleToBig(startLBAAddress));
        console.log("Start Sector:" + little.littleToBig(startLBAAddress) + " (" + parseInt(little.littleToBig(startLBAAddress)) + ")");


        var sizeInSectors = partitionTableEntry.substring(24, 32);
        console.log("Size:" + little.littleToBig(sizeInSectors) + " (" + parseInt(little.littleToBig(sizeInSectors)) + ")");
        console.log("\n");


    }
    return startSectors;

}

function extractRootDir(buffer) {

    // convert the buffer (512 bytes) to hex
    var hex = convert(buffer, {in: 'binary', out: 'hex'});

    // convert hex to an array (mainly so I can visualise it) split on 64 to get all info in one element
    var fileInfoArray = hex.match(/.{1,64}/g);
    //console.log(fileInfoArray);

    fileInfoArray.forEach(function(entry) {

        //finds deleted files (match e5 at start of line)
        if (entry.match(/^e5/)){

            //console.log(entry);
            var filename = entry.substring(0,22);

            //convert the resulting hex to ascii
            var buffer = new Buffer (filename, 'hex');
            var ascii = buffer.toString();
            console.log("\nDeleted File Name: " + ascii);

            var clusterNo = entry.substring(52,56);
            clusterNo = parseInt(little.littleToBig(clusterNo));
            console.log("Starting cluster: " + clusterNo);

            var fileSize = entry.substring(56,64);
            fileSize =  parseInt(little.littleToBig(fileSize));
            console.log("File size: " + fileSize + " bytes");

            clusterSectorAddress = clusterTwoLocation + ((clusterNo - 2) * noSectorsPerCluster);
            console.log("CSA: " + clusterSectorAddress);
        }

    });

}

function extractBootSector(buffer,sectorNumber) {

    // convert the buffer (512 bytes) to hex
    var hex = convert(buffer, {in: 'binary', out: 'hex'});

    // convert hex to an array (mainly so I can visualise it)
    var hexArray = hex.match(/.{1,32}/g);
    //console.log(hexArray);

    var sizeOfReservedArea = hexArray[0].substring(28,32);
    sizeOfReservedArea = parseInt(little.littleToBig(sizeOfReservedArea));
    console.log("Reserved area size in sectors: "+ sizeOfReservedArea);

    var sizeOfFATArea = hexArray[1].substring(12,16);
    sizeOfFATArea = parseInt(little.littleToBig(sizeOfFATArea));
    console.log("FAT size in sectors: " + sizeOfFATArea);

    var noOfFATCopies = hexArray[1].substring(0,2);
    noOfFATCopies = parseInt(little.littleToBig(noOfFATCopies));
    console.log("No. of FATs: " + noOfFATCopies);

    var FATAreaSize = sizeOfFATArea * noOfFATCopies;
    console.log("FAT Area: " +FATAreaSize);

    var maxNoOfDirEntries = hexArray[1].substring(2,6);
    maxNoOfDirEntries = parseInt(little.littleToBig(maxNoOfDirEntries));
    console.log("No. of root dir entries: " + maxNoOfDirEntries);

    var rootDirSizeSectors = maxNoOfDirEntries * 32 / 512;
    console.log("Root dir size in sectors: " + rootDirSizeSectors);

    noSectorsPerCluster = hexArray[0].substring(26,28);
    noSectorsPerCluster = parseInt(little.littleToBig(noSectorsPerCluster));
    console.log("No. of sectors per cluster: " + noSectorsPerCluster);

    var addressOfDataArea = sectorNumber + sizeOfReservedArea + (sizeOfFATArea * sizeOfReservedArea);
    console.log("Sector address of data area (root directory): " + addressOfDataArea);

    clusterTwoLocation = addressOfDataArea + rootDirSizeSectors;
    console.log("Cluster #2 location: " + clusterTwoLocation);

    return addressOfDataArea;
}

// Function to get the contents of a deleted file
function extractDeletedInfo(buffer) {

    // convert the buffer (512 bytes) to hex
    var hex = convert(buffer, {in: 'binary', out: 'hex'});

    // convert hex to an array (mainly so I can visualise it) split on 32 to get all info in one element
    var deletedContents = hex.match(/.{1,32}/g);

    //Creates a buffer and puts the first element in the deletedContents array inside it.
    var deletedBuffer = new Buffer (deletedContents[0],'hex');

    //converts the contents of deletedBuffer to a string.
    var ascii = deletedBuffer.toString();
    console.log("Recovered Data from deleted file: "+ ascii);

}








