module.exports = {
//Simple switch function to convert the type of partition to a string
    partitionType: function (typeOfPartition) {

        var typeToText;
        switch (typeOfPartition) {
            case '00':
                typeToText = "Unknown or unassigned";
                break;
            case '01':
                typeToText = "12-bit FAT";
                break;
            case '04':
                typeToText = "16-bit FAT (<32MB)";
                break;
            case '05':
                typeToText = "Extended MS-DOS Partition";
                break;
            case '06':
                typeToText = "FAT-16 (32MB to 2GB)";
                break;
            case '07':
                typeToText = "NTFS";
                break;
            case '0b':
                typeToText = "FAT-32 (CHS)";
                break;
            case '0c':
                typeToText = "FAT-32 (LBA)";
                break;
            case '0e':
                typeToText = "FAT-16 (LBA)";
                break;
            default:
                typeToText = "Misc";
                break;

        }
        return " (" + typeToText + ")";
    },

    isPartitionActive: function (flag) {

        var typeToText;
        switch (flag) {
            case '00':
                typeToText = "Inactive";
                break;
            case '80':
                typeToText = "Active";
                break;
            default:
                typeToText = "Unknown";
                break;
        }

        return "(" + typeToText + ")";
    }
};