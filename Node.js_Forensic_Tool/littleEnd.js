module.exports = {


//Function takes in a little endian number and returns the same number as big endian
    littleToBig: function (littleEndianNumber) {

        var littleEndianPairs = littleEndianNumber.match(/../g);             // split number in groups of two
        littleEndianPairs.reverse();                                     // reverse the groups
        var bigEnd = littleEndianPairs.join("");

        //Converts bigHex to uppercase then tacks on "0x"
        var bigToHex = ("0x" + bigEnd.toUpperCase());

        return bigToHex;
    }
};