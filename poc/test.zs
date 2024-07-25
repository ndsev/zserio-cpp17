subtype bit:5 CoordWidth;
subtype bit:5 CoordShift;

subtype varsize LinePosition;

subtype int32 Longitude;
subtype Longitude DeltaLongitude;

subtype int32 Latitude;
subtype Latitude DeltaLatitude;

struct RouteSegment
{
    bool hasEndOffset;
    CoordShift shift : shift > 0;
    uint32 myOffset;
    PositionContainer<LinePositionOffset2D, int32>(shift) container;
    packed PositionContainer<LinePositionOffset2D, string>(shift) packedContainer[3];
myOffset:
    LinePositionOffset2D(shift) endPositionWithOffset if hasEndOffset;
    packed int32 myArray[3];
    BoolParamChoice(hasEndOffset) myChoice;
    SimpleUnion myUnion;
};

struct PositionContainer<T, U>(CoordShift shift)
{
    varsize numElements;
    T(shift) array[numElements];
    U field;
};

struct LinePositionOffset2D(CoordShift shift)
{
    LinePosition linePosition;
    CoordWidth numBits;

    PositionOffset2D(numBits, shift) offset;
};

struct PositionOffset2D(CoordWidth numBits, CoordShift shift)
{
    int<numBits + 1> deltaLongitude;
    int<numBits + 1> deltaLatitude;

    function DeltaLongitude lon()
    {
        return deltaLongitude << shift;
    }

    function DeltaLatitude lat()
    {
        return deltaLatitude << shift;
    }
};

choice BoolParamChoice(bool tag) on tag
{
    case false:
        int8 valueA;

    case true:
        int16 valueB;
};

union SimpleUnion
{
    string  valueA;
    int8    valueB;
};