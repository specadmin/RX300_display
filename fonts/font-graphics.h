//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
const BYTE PROGMEM fontGraphicsChars[] =
{

// blank
_1BIT_FONT(
    12,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000),

// 12x18 full fillment
_1BIT_FONT(
    12,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111),

// 6px empty rows
_1BIT_FONT(
    12,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111),

// outer circle D=25px bottom left
_1BIT_FONT(
    12,
    1000,0000,0000,
    1000,0000,0000,
    1000,0000,0000,
    1100,0000,0000,
    1100,0000,0000,
    1110,0000,0000,
    1110,0000,0000,
    1111,0000,0000,
    1111,1000,0000,
    1111,1110,0000,
    1111,1111,1000,
    1111,1111,1111,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000),

// outer circle D=25px bottom right
_1BIT_FONT(
    12,
    0000,0000,0001,
    0000,0000,0001,
    0000,0000,0001,
    0000,0000,0011,
    0000,0000,0011,
    0000,0000,0111,
    0000,0000,0111,
    0000,0000,1111,
    0000,0001,1111,
    0000,0111,1111,
    0001,1111,1111,
    1111,1111,1111,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000),

// outer circle D=25px top left
_1BIT_FONT(
    12,
    1111,1111,1111,
    1111,1111,1000,
    1111,1110,0000,
    1111,1000,0000,
    1111,0000,0000,
    1110,0000,0000,
    1110,0000,0000,
    1100,0000,0000,
    1100,0000,0000,
    1000,0000,0000,
    1000,0000,0000,
    1000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000),

// outer circle D=25px top right
_1BIT_FONT(
    12,
    1111,1111,1111,
    0001,1111,1111,
    0000,0111,1111,
    0000,0001,1111,
    0000,0000,1111,
    0000,0000,0111,
    0000,0000,0111,
    0000,0000,0011,
    0000,0000,0011,
    0000,0000,0001,
    0000,0000,0001,
    0000,0000,0001,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000),

// outer circle D=8px bottom left
_1BIT_FONT(
    12,
    1111,1111,1111,
    0111,1111,1111,
    0111,1111,1111,
    0001,1111,1111,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000),

// outer circle D=8px bottom right
_1BIT_FONT(
    12,
    1111,1111,1111,
    1111,1111,1110,
    1111,1111,1110,
    1111,1111,1000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000),

// outer circle D=8px top left
_1BIT_FONT(
    12,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0001,1111,1111,
    0111,1111,1111,
    0111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000),

_1BIT_FONT(  // outer circle D=8px top right
    12,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    1111,1111,1000,
    1111,1111,1110,
    1111,1111,1110,
    1111,1111,1111,
    1111,1111,1111,
    1111,1111,1111,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000),

// degree symbol  9x7
_1BIT_FONT(          //      123456789012
    9,               //      ------------
    0000,0111,0000,  //  1  |     000    |
    0000,1111,1000,  //  2  |    00000   |
    0001,1000,1100,  //  3  |   00   00  |
    0001,1000,1100,  //  4  |   00   00  |
    0001,1000,1100,  //  5  |   00   00  |
    0000,1111,1000,  //  6  |    00000   |
    0000,0111,0000,  //  7  |     000    |
    0000,0000,0000,  //  8  |            |
    0000,0000,0000,  //  9  |            |
    0000,0000,0000,  // 10  |            |
    0000,0000,0000,  // 11  |            |
    0000,0000,0000,  // 12  |            |
    0000,0000,0000,  // 13  |            |
    0000,0000,0000,  // 14  |            |
    0000,0000,0000,  // 15  |            |
    0000,0000,0000,  // 16  |            |
    0000,0000,0000,  // 17  |            |
    0000,0000,0000), // 18  |            |
                     //      ------------

// 'C'  11x16
_1BIT_FONT(          //      123456789012
    10,              //      ------------
    0000,1111,1000,  //  1  |    00000   |
    0001,1111,1100,  //  2  |   0000000  |
    0011,0000,0110,  //  3  |  00     00 |
    0011,0000,0110,  //  4  |  00     00 |
    0110,0000,0010,  //  5  | 00       0 |
    0110,0000,0000,  //  6  | 00         |
    0110,0000,0000,  //  7  | 00         |
    0110,0000,0000,  //  8  | 00         |
    0110,0000,0000,  //  9  | 00         |
    0110,0000,0000,  // 10  | 00         |
    0110,0000,0000,  // 11  | 00         |
    0110,0000,0010,  // 12  | 00       0 |
    0011,0000,0110,  // 13  |  00     00 |
    0011,0000,0110,  // 14  |  00     00 |
    0001,1111,1100,  // 15  |   0000000  |
    0000,1111,1000,  // 16  |    00000   |
    0000,0000,0000,  // 17  |            |
    0000,0000,0000), // 18  |            |
                     //      ------------

// 'F'  8x16
_1BIT_FONT(          //      123456789012
    8,               //      ------------
    0011,1111,1100,  //  1  |  00000000  |
    0011,1111,1100,  //  2  |  00000000  |
    0011,0000,0000,  //  3  |  00        |
    0011,0000,0000,  //  4  |  00        |
    0011,0000,0000,  //  5  |  00        |
    0011,0000,0000,  //  6  |  00        |
    0011,0000,0000,  //  7  |  00        |
    0011,1111,1100,  //  8  |  00000000  |
    0011,1111,1100,  //  9  |  00000000  |
    0011,0000,0000,  // 10  |  00        |
    0011,0000,0000,  // 11  |  00        |
    0011,0000,0000,  // 12  |  00        |
    0011,0000,0000,  // 13  |  00        |
    0011,0000,0000,  // 14  |  00        |
    0011,0000,0000,  // 15  |  00        |
    0011,0000,0000,  // 16  |  00        |
    0000,0000,0000,  // 17  |            |
    0000,0000,0000), // 18  |            |
                     //      ------------

// narrow 'k'
_1BIT_FONT(
    7,
    0001,0000,0000,
    0001,0000,0000,
    0001,0000,0000,
    0001,0000,0000,
    0001,0000,0000,
    0001,0000,1000,
    0001,0001,0000,
    0001,0010,0000,
    0001,0100,0000,
    0001,1100,0000,
    0001,0010,0000,
    0001,0001,0000,
    0001,0001,0000,
    0001,0000,1000,
    0001,0000,1000,
    0001,0000,0100,
    0000,0000,0000,
    0000,0000,0000),

// narrow  'm'
_1BIT_FONT(
    11,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0000,0000,0000,
    0011,1101,1110,
    0100,1010,0001,
    0100,0010,0001,
    0100,0010,0001,
    0100,0010,0001,
    0100,0010,0001,
    0100,0010,0001,
    0100,0010,0001,
    0100,0010,0001,
    0100,0010,0001,
    0100,0010,0001,
    0000,0000,0000,
    0000,0000,0000),

// narrow '/'
_1BIT_FONT(
    5,
    0000,0000,1000,
    0000,0001,0000,
    0000,0001,0000,
    0000,0001,0000,
    0000,0001,0000,
    0000,0010,0000,
    0000,0010,0000,
    0000,0010,0000,
    0000,0010,0000,
    0000,0100,0000,
    0000,0100,0000,
    0000,0100,0000,
    0000,0100,0000,
    0000,1000,0000,
    0000,1000,0000,
    0000,1000,0000,
    0000,0000,0000,
    0000,0000,0000),

// narrow  'h'
_1BIT_FONT(
    7,
    0001,0000,0000,
    0001,0000,0000,
    0001,0000,0000,
    0001,0000,0000,
    0001,0000,0000,
    0001,0111,1000,
    0001,1000,0100,
    0001,0000,0100,
    0001,0000,0100,
    0001,0000,0100,
    0001,0000,0100,
    0001,0000,0100,
    0001,0000,0100,
    0001,0000,0100,
    0001,0000,0100,
    0001,0000,0100,
    0000,0000,0000,
    0000,0000,0000),
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------------
