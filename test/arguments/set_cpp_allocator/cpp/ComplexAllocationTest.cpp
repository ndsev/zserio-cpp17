#include <array>
#include <memory>
#include <string>

#include "complex_allocation/MainStructure.h"
#include "gtest/gtest.h"
#include "test_utils/MemoryResources.h"
#include "test_utils/TestUtility.h"
#include "zserio/BitStreamReader.h"
#include "zserio/BitStreamWriter.h"

using namespace std::literals;
using namespace test_utils;

namespace complex_allocation
{

using allocator_type = MainStructure::allocator_type;
using StringType = zserio::BasicString<zserio::RebindAlloc<allocator_type, char>>;
template <typename T>
using VectorType = zserio::Vector<T, zserio::RebindAlloc<allocator_type, T>>;

using BitBufferType = zserio::BasicBitBuffer<zserio::RebindAlloc<allocator_type, uint8_t>>;

class ComplexAllocationTest : public ::testing::Test
{
protected:
    void writeData(zserio::BitStreamWriter& writer, bool hasArray)
    {
        // stringField
        writer.writeString(StringType(STRING_FIELD));

        // stringArray[]
        writer.writeVarSize(STRING_ARRAY_SIZE);
        writer.writeString(StringType(STRING_ARRAY_ELEMENT0));
        writer.writeString(StringType(STRING_ARRAY_ELEMENT1));
        writer.writeString(StringType(STRING_ARRAY_ELEMENT2));

        // hasArray
        writer.writeBool(hasArray);

        // choiceField
        if (hasArray)
        {
            writer.writeVarSize(CHOICE_COMPOUND_ARRAY_SIZE);
            writer.writeUnsignedBits32(CHOICE_COMPOUND_ELEMENT0_VALUE16, 16);
            writer.writeBool(CHOICE_COMPOUND_ELEMENT0_IS_VALID);
            writer.writeUnsignedBits32(CHOICE_COMPOUND_ELEMENT1_VALUE16, 16);
            writer.writeBool(CHOICE_COMPOUND_ELEMENT1_IS_VALID);
        }
        else
        {
            writer.writeUnsignedBits32(CHOICE_COMPOUND_ELEMENT0_VALUE16, 16);
            writer.writeBool(CHOICE_COMPOUND_ELEMENT0_IS_VALID);
        }

        // unionField
        if (hasArray)
        {
            writer.writeVarSize(static_cast<uint32_t>(allocation_union::AllocationUnion::Tag::array) - 1);
            writer.writeVarSize(UNION_COMPOUND_ARRAY_SIZE);
        }
        else
        {
            writer.writeVarSize(static_cast<uint32_t>(allocation_union::AllocationUnion::Tag::compound) - 1);
        }
        writer.writeUnsignedBits32(UNION_COMPOUND_ELEMENT0_VALUE16, 16);
        writer.writeBool(UNION_COMPOUND_ELEMENT0_IS_VALID);

        // structField
        writer.writeVarSize(STRUCT_BIT7_ARRAY_SIZE);
        writer.writeUnsignedBits32(STRUCT_BIT7_ARRAY_ELEMENT0, 7);
        writer.writeUnsignedBits32(STRUCT_BIT7_ARRAY_ELEMENT1, 7);
        writer.writeUnsignedBits32(STRUCT_BIT7_ARRAY_ELEMENT2, 7);
        writer.writeString(StringType(STRUCT_STRING_FIELD));
        writer.writeString(StringType(STRUCT_DEFAULT_STRING_FIELD));
        writer.writeVarSize(STRUCT_PACKED_UINT16_ARRAY_SIZE);
        writer.writeBool(true);
        writer.writeUnsignedBits32(STRUCT_PACKED_UINT16_ARRAY_MAX_BIT_NUMBER, 6);
        writer.writeUnsignedBits32(STRUCT_PACKED_UINT16_ARRAY_ELEMENT0, 16);
        writer.writeSignedBits32(STRUCT_PACKED_UINT16_ARRAY_DELTA,
                static_cast<uint8_t>(STRUCT_PACKED_UINT16_ARRAY_MAX_BIT_NUMBER + 1));
        writer.writeSignedBits32(STRUCT_PACKED_UINT16_ARRAY_DELTA,
                static_cast<uint8_t>(STRUCT_PACKED_UINT16_ARRAY_MAX_BIT_NUMBER + 1));
        writer.writeVarSize(STRUCT_PACKED_ELEMENT_ARRAY_SIZE);
        // x0
        writer.writeBool(true);
        writer.writeUnsignedBits32(STRUCT_PACKED_ELEMENT_ARRAY_MAX_BIT_NUMBER, 6);
        writer.writeUnsignedBits32(STRUCT_PACKED_ELEMENT_ARRAY_ELEMENT0_X, 32);
        // y0
        writer.writeBool(true);
        writer.writeUnsignedBits32(STRUCT_PACKED_ELEMENT_ARRAY_MAX_BIT_NUMBER, 6);
        writer.writeUnsignedBits32(STRUCT_PACKED_ELEMENT_ARRAY_ELEMENT0_Y, 32);
        // z0
        writer.writeBool(true);
        writer.writeUnsignedBits32(STRUCT_PACKED_ELEMENT_ARRAY_MAX_BIT_NUMBER, 6);
        writer.writeUnsignedBits32(STRUCT_PACKED_ELEMENT_ARRAY_ELEMENT0_Z, 32);
        // x1
        writer.writeSignedBits32(STRUCT_PACKED_ELEMENT_ARRAY_DELTA,
                static_cast<uint8_t>(STRUCT_PACKED_ELEMENT_ARRAY_MAX_BIT_NUMBER + 1));
        // y1
        writer.writeSignedBits32(STRUCT_PACKED_ELEMENT_ARRAY_DELTA,
                static_cast<uint8_t>(STRUCT_PACKED_ELEMENT_ARRAY_MAX_BIT_NUMBER + 1));
        // z1
        writer.writeSignedBits32(STRUCT_PACKED_ELEMENT_ARRAY_DELTA,
                static_cast<uint8_t>(STRUCT_PACKED_ELEMENT_ARRAY_MAX_BIT_NUMBER + 1));
        // x2
        writer.writeSignedBits32(STRUCT_PACKED_ELEMENT_ARRAY_DELTA,
                static_cast<uint8_t>(STRUCT_PACKED_ELEMENT_ARRAY_MAX_BIT_NUMBER + 1));
        // y2
        writer.writeSignedBits32(STRUCT_PACKED_ELEMENT_ARRAY_DELTA,
                static_cast<uint8_t>(STRUCT_PACKED_ELEMENT_ARRAY_MAX_BIT_NUMBER + 1));
        // z2
        writer.writeSignedBits32(STRUCT_PACKED_ELEMENT_ARRAY_DELTA,
                static_cast<uint8_t>(STRUCT_PACKED_ELEMENT_ARRAY_MAX_BIT_NUMBER + 1));

        // structOptionalField
        writer.writeBool(true);
        writer.writeVarSize(STRUCT_OPTIONAL_NAMES0_SIZE);
        writer.writeString(StringType(STRUCT_OPTIONAL_NAMES0_ELEMENT0));
        writer.writeString(StringType(STRUCT_OPTIONAL_NAMES0_ELEMENT1));
        writer.writeBool(true);

        writer.writeBool(true);
        writer.writeVarSize(STRUCT_OPTIONAL_NAMES1_SIZE);
        writer.writeString(StringType(STRUCT_OPTIONAL_NAMES1_ELEMENT0));
        writer.writeBool(false);

        // externalField
        writer.writeVarSize(EXTERNAL_FIELD_VAR_SIZE);
        writer.writeUnsignedBits32(
                static_cast<uint32_t>(EXTERNAL_FIELD_DATA) >> (8U - (EXTERNAL_FIELD_VAR_SIZE % 8U)),
                EXTERNAL_FIELD_VAR_SIZE);

        // externalArray
        writer.writeVarSize(EXTERNAL_ARRAY_SIZE);
        writer.writeVarSize(EXTERNAL_ARRAY_ELEMENT0_VAR_SIZE);
        writer.writeUnsignedBits32(static_cast<uint32_t>(EXTERNAL_ARRAY_ELEMENT0_DATA) >>
                        (8U - (EXTERNAL_ARRAY_ELEMENT0_VAR_SIZE % 8U)),
                EXTERNAL_ARRAY_ELEMENT0_VAR_SIZE);
        writer.writeVarSize(EXTERNAL_ARRAY_ELEMENT1_VAR_SIZE);
        writer.writeUnsignedBits32(static_cast<uint32_t>(EXTERNAL_ARRAY_ELEMENT1_DATA) >>
                        (8U - (EXTERNAL_ARRAY_ELEMENT1_VAR_SIZE % 8U)),
                EXTERNAL_ARRAY_ELEMENT1_VAR_SIZE);

        // bytesField
        writer.writeVarSize(BYTES_FIELD_VAR_SIZE);
        writer.writeUnsignedBits32(BYTES_FIELD_DATA, static_cast<uint8_t>(BYTES_FIELD_VAR_SIZE * 8));

        // bytesArray
        writer.writeVarSize(BYTES_ARRAY_SIZE);
        writer.writeVarSize(BYTES_ARRAY_ELEMENT0_VAR_SIZE);
        writer.writeUnsignedBits32(
                BYTES_ARRAY_ELEMENT0_DATA, static_cast<uint8_t>(BYTES_ARRAY_ELEMENT0_VAR_SIZE * 8));
        writer.writeVarSize(BYTES_ARRAY_ELEMENT1_VAR_SIZE);
        writer.writeUnsignedBits32(
                BYTES_ARRAY_ELEMENT1_DATA, static_cast<uint8_t>(BYTES_ARRAY_ELEMENT1_VAR_SIZE * 8));
    }

    void fillStringArray(VectorType<StringType>& stringArray, const allocator_type& allocator)
    {
        stringArray.reserve(STRING_ARRAY_SIZE);
        stringArray.emplace_back(STRING_ARRAY_ELEMENT0, allocator);
        stringArray.emplace_back(STRING_ARRAY_ELEMENT1, allocator);
        stringArray.emplace_back(STRING_ARRAY_ELEMENT2, allocator);
    }

    void fillChoiceField(
            allocation_choice::AllocationChoice& choiceField, const allocator_type& allocator, bool hasArray)
    {
        if (hasArray)
        {
            VectorType<allocation_choice::ChoiceCompound> array(allocator);
            array.reserve(CHOICE_COMPOUND_ARRAY_SIZE);
            array.emplace_back(CHOICE_COMPOUND_ELEMENT0_VALUE16, CHOICE_COMPOUND_ELEMENT0_IS_VALID);
            array.emplace_back(CHOICE_COMPOUND_ELEMENT1_VALUE16, CHOICE_COMPOUND_ELEMENT1_IS_VALID);
            choiceField.emplace<allocation_choice::AllocationChoice::Tag::array>(std::move(array));
        }
        else
        {
            choiceField.emplace<allocation_choice::AllocationChoice::Tag::compound>(
                    allocation_choice::ChoiceCompound(
                            CHOICE_COMPOUND_ELEMENT0_VALUE16, CHOICE_COMPOUND_ELEMENT0_IS_VALID));
        }
    }

    void fillUnionField(
            allocation_union::AllocationUnion& unionField, const allocator_type& allocator, bool hasArray)
    {
        if (hasArray)
        {
            VectorType<allocation_union::UnionCompound> array(allocator);
            array.reserve(UNION_COMPOUND_ARRAY_SIZE);
            array.emplace_back(UNION_COMPOUND_ELEMENT0_VALUE16, UNION_COMPOUND_ELEMENT0_IS_VALID);
            unionField.emplace<allocation_union::AllocationUnion::Tag::array>(std::move(array));
        }
        else
        {
            unionField.emplace<allocation_union::AllocationUnion::Tag::compound>(
                    allocation_union::UnionCompound(
                            UNION_COMPOUND_ELEMENT0_VALUE16, UNION_COMPOUND_ELEMENT0_IS_VALID));
        }
    }

    void fillStructField(allocation_struct::AllocationStruct& structField)
    {
        auto& bit7Array = structField.bit7Array;
        bit7Array.reserve(STRUCT_BIT7_ARRAY_SIZE);
        bit7Array.push_back(STRUCT_BIT7_ARRAY_ELEMENT0);
        bit7Array.push_back(STRUCT_BIT7_ARRAY_ELEMENT1);
        bit7Array.push_back(STRUCT_BIT7_ARRAY_ELEMENT2);
        auto& stringField = structField.stringField;
        stringField.assign(STRUCT_STRING_FIELD);
        auto& packedUInt16Array = structField.packedUInt16Array;
        packedUInt16Array.reserve(STRUCT_PACKED_UINT16_ARRAY_SIZE);
        packedUInt16Array.push_back(STRUCT_PACKED_UINT16_ARRAY_ELEMENT0);
        packedUInt16Array.push_back(STRUCT_PACKED_UINT16_ARRAY_ELEMENT1);
        packedUInt16Array.push_back(STRUCT_PACKED_UINT16_ARRAY_ELEMENT2);
        auto& packedElementArray = structField.packedElementArray;
        packedElementArray.reserve(STRUCT_PACKED_ELEMENT_ARRAY_SIZE);
        packedElementArray.emplace_back(STRUCT_PACKED_ELEMENT_ARRAY_ELEMENT0_X,
                STRUCT_PACKED_ELEMENT_ARRAY_ELEMENT0_Y, STRUCT_PACKED_ELEMENT_ARRAY_ELEMENT0_Z);
        packedElementArray.emplace_back(STRUCT_PACKED_ELEMENT_ARRAY_ELEMENT0_X +
                        static_cast<uint32_t>(STRUCT_PACKED_ELEMENT_ARRAY_DELTA),
                STRUCT_PACKED_ELEMENT_ARRAY_ELEMENT0_Y +
                        static_cast<uint32_t>(STRUCT_PACKED_ELEMENT_ARRAY_DELTA),
                STRUCT_PACKED_ELEMENT_ARRAY_ELEMENT0_Z +
                        static_cast<uint32_t>(STRUCT_PACKED_ELEMENT_ARRAY_DELTA));
        packedElementArray.emplace_back(STRUCT_PACKED_ELEMENT_ARRAY_ELEMENT0_X +
                        static_cast<uint32_t>(STRUCT_PACKED_ELEMENT_ARRAY_DELTA) * 2,
                STRUCT_PACKED_ELEMENT_ARRAY_ELEMENT0_Y +
                        static_cast<uint32_t>(STRUCT_PACKED_ELEMENT_ARRAY_DELTA) * 2,
                STRUCT_PACKED_ELEMENT_ARRAY_ELEMENT0_Z +
                        static_cast<uint32_t>(STRUCT_PACKED_ELEMENT_ARRAY_DELTA) * 2);
    }

    void fillOptionalField(allocation_struct_optional::AllocationStructOptional& structOptionalField,
            const allocator_type& allocator)
    {
        VectorType<StringType> names0(allocator);
        names0.reserve(STRUCT_OPTIONAL_NAMES0_SIZE);
        names0.emplace_back(STRUCT_OPTIONAL_NAMES0_ELEMENT0, allocator);
        names0.emplace_back(STRUCT_OPTIONAL_NAMES0_ELEMENT1, allocator);
        structOptionalField.names = std::move(names0);
        structOptionalField.hasNext = true;

        allocation_struct_optional::AllocationStructOptional others(allocator);
        VectorType<StringType> names1(allocator);
        names1.reserve(STRUCT_OPTIONAL_NAMES1_SIZE);
        names1.emplace_back(STRUCT_OPTIONAL_NAMES1_ELEMENT0, allocator);
        others.names = std::move(names1);
        others.hasNext = false;
        structOptionalField.others = std::move(others);
    }

    BitBufferType createExternalField(const allocator_type& allocator)
    {
        const std::array<uint8_t, 2> externalFieldData = {
                static_cast<uint8_t>(EXTERNAL_FIELD_DATA >> 8U), static_cast<uint8_t>(EXTERNAL_FIELD_DATA)};

        return BitBufferType(externalFieldData, EXTERNAL_FIELD_VAR_SIZE, allocator);
    }

    void fillExternalArray(VectorType<BitBufferType>& externalArray, const allocator_type& allocator)
    {
        externalArray.reserve(EXTERNAL_ARRAY_SIZE);
        externalArray.emplace_back(
                zserio::Span<const uint8_t>(
                        &EXTERNAL_ARRAY_ELEMENT0_DATA, EXTERNAL_ARRAY_ELEMENT0_VAR_SIZE + 7 / 8),
                EXTERNAL_ARRAY_ELEMENT0_VAR_SIZE, allocator);
        const std::array<uint8_t, 2> externalElement1Data = {
                static_cast<uint8_t>(EXTERNAL_ARRAY_ELEMENT1_DATA >> 8U),
                static_cast<uint8_t>(EXTERNAL_ARRAY_ELEMENT1_DATA)};
        externalArray.emplace_back(externalElement1Data, EXTERNAL_ARRAY_ELEMENT1_VAR_SIZE, allocator);
    }

    VectorType<uint8_t> createBytesField(const allocator_type& allocator)
    {
        return VectorType<uint8_t>(
                {static_cast<uint8_t>(BYTES_FIELD_DATA >> 8U), static_cast<uint8_t>(BYTES_FIELD_DATA)},
                allocator);
    }

    void fillBytesArray(VectorType<VectorType<uint8_t>>& bytesArray, const allocator_type& allocator)
    {
        bytesArray.reserve(BYTES_ARRAY_SIZE);
        const std::array<uint8_t, 2> bytesArrayElement0Data = {
                static_cast<uint8_t>(BYTES_ARRAY_ELEMENT0_DATA >> 8U),
                static_cast<uint8_t>(BYTES_ARRAY_ELEMENT0_DATA)};
        bytesArray.emplace_back(bytesArrayElement0Data.begin(), bytesArrayElement0Data.end(), allocator);
        bytesArray.emplace_back(BYTES_ARRAY_ELEMENT1_VAR_SIZE, BYTES_ARRAY_ELEMENT1_DATA, allocator);
    }

    void fillData(MainStructure& data, const allocator_type& allocator, bool hasArray)
    {
        // stringField
        data.stringField = STRING_FIELD;

        // stringArray[]
        fillStringArray(data.stringArray, allocator);

        // hasArray
        data.hasArray = hasArray;

        // choiceField
        fillChoiceField(data.choiceField, allocator, hasArray);

        // unionField
        fillUnionField(data.unionField, allocator, hasArray);

        // structField
        fillStructField(data.structField);

        // structOptionalField
        fillOptionalField(data.structOptionalField, allocator);

        // externalField
        data.externalField = createExternalField(allocator);

        // externalArray
        fillExternalArray(data.externalArray, allocator);

        // bytesField
        data.bytesField = createBytesField(allocator);

        // bytesArray
        fillBytesArray(data.bytesArray, allocator);
    }

    void checkRead(bool hasArray)
    {
        MainStructure expectedReadData;
        fillData(expectedReadData, allocator_type(), hasArray);
        zserio::View expectedReadView(expectedReadData);

        const zserio::View view(expectedReadData);
        const zserio::BitSize bitSize = zserio::detail::bitSizeOf(view);
        zserio::BitBuffer bitBuffer(bitSize);
        zserio::BitStreamWriter writer(bitBuffer);
        writeData(writer, hasArray);

        // check that default memory resource won't be used
        InvalidMemoryResource invalidMemoryResource;
        MemoryResourceScopedSetter invalidMemoryResourceScopedSetter(invalidMemoryResource);

        zserio::BitStreamReader reader(writer.getWriteBuffer(), writer.getBitPosition(), zserio::BitsTag());
        TestMemoryResource<> memoryResource("Memory Resource #1");
        {
            const allocator_type allocator(&memoryResource);
            MainStructure readData(allocator);
            zserio::View readView = zserio::detail::read(reader, readData);

            ASSERT_EQ(expectedReadData, readData);
            ASSERT_EQ(expectedReadView, readView);

            // check memory fragmentation in used memory resource
            // TODO[Mi-L@]: Dynamically allocated default values (i.e. strings) cause deallocation during read!
            ASSERT_EQ(1, memoryResource.getNumDeallocations());
        }

        // check memory leaks in used memory resource
        ASSERT_EQ(memoryResource.getNumDeallocations(), memoryResource.getNumAllocations());
    }

    void checkCopyConstructor(bool hasArray)
    {
        // if allocator is propagating, use invalid memory resource
        const bool hasPropagatingAllocator = hasCopyPropagatingAllocator();
        HeapMemoryResource heapMemoryResource;
        InvalidMemoryResource invalidMemoryResource;
        MemoryResourceScopedSetter memoryResourceScopedSetter(hasPropagatingAllocator
                        ? static_cast<std::pmr::memory_resource&>(invalidMemoryResource)
                        : static_cast<std::pmr::memory_resource&>(heapMemoryResource));

        TestMemoryResource<1024 * 4> memoryResource("Memory Resource #1");
        {
            const allocator_type allocator(&memoryResource);
            MainStructure data(allocator);
            fillData(data, allocator, hasArray);

            // call copy constructor
            const size_t numAllocations = memoryResource.getNumAllocations();
            const size_t allocatedSize = memoryResource.getAllocatedSize();
            MainStructure dataCopy(data);
            ASSERT_EQ(data, dataCopy);

            // check that copy has allocated the same memory as the original
            size_t totalNumAllocations = memoryResource.getNumAllocations();
            size_t totalAllocatedSize = memoryResource.getAllocatedSize();
            if (!hasPropagatingAllocator)
            {
                totalNumAllocations += heapMemoryResource.getNumAllocations();
                totalAllocatedSize += heapMemoryResource.getAllocatedSize();
            }
            ASSERT_EQ(numAllocations * 2, totalNumAllocations);
            ASSERT_EQ(allocatedSize * 2, totalAllocatedSize);

            // check memory fragmentation in used memory resources
            ASSERT_EQ(0, memoryResource.getNumDeallocations());
            if (!hasPropagatingAllocator)
            {
                ASSERT_EQ(0, heapMemoryResource.getNumDeallocations());
            }
        }

        // check memory leaks in used memory resources
        ASSERT_EQ(memoryResource.getNumDeallocations(), memoryResource.getNumAllocations());
        if (!hasPropagatingAllocator)
        {
            ASSERT_EQ(heapMemoryResource.getNumDeallocations(), heapMemoryResource.getNumAllocations());
        }
    }

    void checkMoveConstructor(bool hasArray)
    {
        // check that default memory resource won't be used
        InvalidMemoryResource invalidMemoryResource;
        MemoryResourceScopedSetter invalidMemoryResourceScopedSetter(invalidMemoryResource);

        TestMemoryResource<> memoryResource("Memory Resource #1");
        {
            const allocator_type allocator(&memoryResource);
            MainStructure data(allocator);
            fillData(data, allocator, hasArray);

            // call move constructor
            const size_t numAllocations = memoryResource.getNumAllocations();
            MainStructure dataMove(std::move(data));

            // check that move constructor does not allocate anything
            ASSERT_EQ(numAllocations, memoryResource.getNumAllocations());

            // check memory fragmentation in used memory resource
            ASSERT_EQ(0, memoryResource.getNumDeallocations());
        }

        // check memory leaks in used memory resource
        ASSERT_EQ(memoryResource.getNumDeallocations(), memoryResource.getNumAllocations());
    }

    void checkCopyAssignmentOperator(bool hasArray)
    {
        // if allocator is propagating, use invalid memory resource
        const bool hasPropagatingAllocator =
                std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value;

        InvalidMemoryResource invalidMemoryResource;
        HeapMemoryResource heapMemoryResource;
        MemoryResourceScopedSetter memoryResourceScopedSetter(hasPropagatingAllocator
                        ? static_cast<std::pmr::memory_resource&>(invalidMemoryResource)
                        : static_cast<std::pmr::memory_resource&>(heapMemoryResource));

        TestMemoryResource<1024 * 4> memoryResource("Memory Resource #1");
        {
            const allocator_type allocator(&memoryResource);
            MainStructure data(allocator);
            fillData(data, allocator, hasArray);

            // call copy assignment operator
            const size_t numAllocations = memoryResource.getNumAllocations();
            const size_t allocatedSize = memoryResource.getAllocatedSize();
            MainStructure dataCopyAssignment = data;
            ASSERT_EQ(data, dataCopyAssignment);

            // check that copy has been allocated the same memory as the original
            size_t totalNumAllocations = memoryResource.getNumAllocations();
            size_t totalAllocatedSize = memoryResource.getAllocatedSize();
            if (!hasPropagatingAllocator)
            {
                totalNumAllocations += heapMemoryResource.getNumAllocations();
                totalAllocatedSize += heapMemoryResource.getAllocatedSize();
            }
            ASSERT_EQ(numAllocations * 2, totalNumAllocations);
            ASSERT_EQ(allocatedSize * 2, totalAllocatedSize);

            // check memory fragmentation in used memory resources
            ASSERT_EQ(0, memoryResource.getNumDeallocations());
            if (!hasPropagatingAllocator)
            {
                ASSERT_EQ(0, heapMemoryResource.getNumDeallocations());
            }
        }

        // check memory leaks in used memory resources
        ASSERT_EQ(memoryResource.getNumDeallocations(), memoryResource.getNumAllocations());
        if (!hasPropagatingAllocator)
        {
            ASSERT_EQ(heapMemoryResource.getNumDeallocations(), heapMemoryResource.getNumAllocations());
        }
    }

    void checkMoveAssignmentOperator(bool hasArray)
    {
        // check that default memory resource won't be used
        InvalidMemoryResource invalidMemoryResource;
        MemoryResourceScopedSetter invalidMemoryResourceScopedSetter(invalidMemoryResource);

        TestMemoryResource<> memoryResource("Memory Resource #1");
        {
            const allocator_type allocator(&memoryResource);
            MainStructure data(allocator);
            fillData(data, allocator, hasArray);

            // call move assignment operator
            const size_t numAllocations = memoryResource.getNumAllocations();
            MainStructure dataMoveAssignment = std::move(data);

            // check that move assignment operator does not allocate anything
            ASSERT_EQ(numAllocations, memoryResource.getNumAllocations());

            // check memory fragmentation in used memory resource
            ASSERT_EQ(0, memoryResource.getNumDeallocations());
        }

        // check memory leaks in used memory resource
        ASSERT_EQ(memoryResource.getNumDeallocations(), memoryResource.getNumAllocations());
    }

    void checkAllocatorConstructor(bool hasArray)
    {
        // check that default memory resource won't be used
        InvalidMemoryResource invalidMemoryResource;
        MemoryResourceScopedSetter invalidMemoryResourceScopedSetter(invalidMemoryResource);

        TestMemoryResource<> memoryResource("Memory Resource #1");
        {
            const allocator_type allocator(&memoryResource);
            MainStructure data(allocator);
            fillData(data, allocator, hasArray);

            // check memory fragmentation in used memory resource
            ASSERT_EQ(0, memoryResource.getNumDeallocations());
        }

        // check memory leaks in used memory resource
        ASSERT_EQ(memoryResource.getNumDeallocations(), memoryResource.getNumAllocations());
    }

    void checkFieldConstructor(bool hasArray)
    {
        MainStructure expectedData;
        fillData(expectedData, allocator_type(), hasArray);

        // check that default memory resource won't be used
        InvalidMemoryResource invalidMemoryResource;
        MemoryResourceScopedSetter invalidMemoryResourceScopedSetter(invalidMemoryResource);

        TestMemoryResource<1024 * 3> memoryResource("Memory Resource #1");
        {
            const allocator_type allocator(&memoryResource);

            // string field
            StringType stringField(STRING_FIELD, allocator);

            // stringArray
            VectorType<StringType> stringArray(allocator);
            fillStringArray(stringArray, allocator);

            // choiceField
            allocation_choice::AllocationChoice choiceField(allocator);
            fillChoiceField(choiceField, allocator, hasArray);

            // unionField
            allocation_union::AllocationUnion unionField(allocator);
            fillUnionField(unionField, allocator, hasArray);

            // structField
            allocation_struct::AllocationStruct structField(allocator);
            fillStructField(structField);

            // structOptionalField
            allocation_struct_optional::AllocationStructOptional structOptionalField(allocator);
            fillOptionalField(structOptionalField, allocator);

            // externalArray
            VectorType<BitBufferType> externalArray(allocator);
            fillExternalArray(externalArray, allocator);

            // bytesArray
            VectorType<VectorType<uint8_t>> bytesArray(allocator);
            fillBytesArray(bytesArray, allocator);

            if (hasCopyPropagatingAllocator())
            {
                MainStructure data(stringField, stringArray, hasArray, choiceField, unionField, structField,
                        structOptionalField, createExternalField(allocator), externalArray,
                        createBytesField(allocator), bytesArray);
                ASSERT_EQ(expectedData, data);

                // check memory fragmentation in used memory resource
                ASSERT_EQ(0, memoryResource.getNumDeallocations());
            }
            else
            {
                MainStructure data(stringField, std::move(stringArray), hasArray, std::move(choiceField),
                        std::move(unionField), std::move(structField), std::move(structOptionalField),
                        createExternalField(allocator), std::move(externalArray), createBytesField(allocator),
                        std::move(bytesArray));
                ASSERT_EQ(expectedData, data);

                // check memory fragmentation in used memory resource
                ASSERT_EQ(0, memoryResource.getNumDeallocations());
            }
        }

        // check memory leaks in used memory resource
        ASSERT_EQ(memoryResource.getNumDeallocations(), memoryResource.getNumAllocations());
    }

    void checkBitSizeOf(bool hasArray)
    {
        // check that default memory resource won't be used
        InvalidMemoryResource invalidMemoryResource;
        MemoryResourceScopedSetter invalidMemoryResourceScopedSetter(invalidMemoryResource);

        TestMemoryResource<> memoryResource("Memory Resource #1");
        {
            const allocator_type allocator(&memoryResource);
            MainStructure data(allocator);
            fillData(data, allocator, hasArray);

            zserio::View view(data);

            // check that no further memory allocation will take place
            const size_t numAllocations = memoryResource.getNumAllocations();
            const size_t allocatedSize = memoryResource.getAllocatedSize();
            const size_t expectedBitSize =
                    (hasArray) ? MAIN_STRUCTURE_WITH_ARRAYS_BIT_SIZE : MAIN_STRUCTURE_WITHOUT_ARRAYS_BIT_SIZE;
            ASSERT_EQ(expectedBitSize, zserio::detail::bitSizeOf(view));
            ASSERT_EQ(numAllocations, memoryResource.getNumAllocations());
            ASSERT_EQ(allocatedSize, memoryResource.getAllocatedSize());

            // check memory fragmentation in used memory resource
            ASSERT_EQ(0, memoryResource.getNumDeallocations());
        }

        // check memory leaks in used memory resource
        ASSERT_EQ(memoryResource.getNumDeallocations(), memoryResource.getNumAllocations());
    }

    void checkComparisonOperators(bool hasArray)
    {
        // check that default memory resource won't be used
        InvalidMemoryResource invalidMemoryResource;
        MemoryResourceScopedSetter invalidMemoryResourceScopedSetter(invalidMemoryResource);

        TestMemoryResource<> memoryResource1("Memory Resource #1");
        TestMemoryResource<> memoryResource2("Memory Resource #2");
        TestMemoryResource<> memoryResource3("Memory Resource #3");
        {
            const allocator_type allocator1(&memoryResource1);
            MainStructure data(allocator1);
            fillData(data, allocator1, hasArray);

            const allocator_type allocator2(&memoryResource2);
            MainStructure equalData(allocator2);
            fillData(equalData, allocator2, hasArray);

            const allocator_type allocator3(&memoryResource3);
            MainStructure lessThanData(allocator3);
            fillData(lessThanData, allocator3, hasArray);
            lessThanData.structField.bit7Array.at(0) = STRUCT_BIT7_ARRAY_ELEMENT0 - 1;

            zserio::View view(data);
            zserio::View equalView(equalData);
            zserio::View lessThanView(lessThanData);

            // check that no further memory allocation will take place
            const size_t numAllocations1 = memoryResource1.getNumAllocations();
            const size_t allocatedSize1 = memoryResource1.getAllocatedSize();
            const size_t numAllocations2 = memoryResource2.getNumAllocations();
            const size_t allocatedSize2 = memoryResource2.getAllocatedSize();
            const size_t numAllocations3 = memoryResource3.getNumAllocations();
            const size_t allocatedSize3 = memoryResource3.getAllocatedSize();

            test_utils::comparisonOperatorsTest(data, equalData, lessThanData);
            test_utils::comparisonOperatorsTest(view, equalView, lessThanView);

            ASSERT_EQ(numAllocations1, memoryResource1.getNumAllocations());
            ASSERT_EQ(allocatedSize1, memoryResource1.getAllocatedSize());
            ASSERT_EQ(numAllocations2, memoryResource2.getNumAllocations());
            ASSERT_EQ(allocatedSize2, memoryResource2.getAllocatedSize());
            ASSERT_EQ(numAllocations3, memoryResource3.getNumAllocations());
            ASSERT_EQ(allocatedSize3, memoryResource3.getAllocatedSize());

            // check memory fragmentation in used memory resources
            ASSERT_EQ(0, memoryResource1.getNumDeallocations());
            ASSERT_EQ(0, memoryResource2.getNumDeallocations());
            ASSERT_EQ(0, memoryResource3.getNumDeallocations());
        }

        // check memory leaks in used memory resources
        ASSERT_EQ(memoryResource1.getNumDeallocations(), memoryResource1.getNumAllocations());
        ASSERT_EQ(memoryResource2.getNumDeallocations(), memoryResource2.getNumAllocations());
        ASSERT_EQ(memoryResource3.getNumDeallocations(), memoryResource3.getNumAllocations());
    }
    void checkStdHash(bool hasArray)
    {
        // check that default memory resource won't be used
        InvalidMemoryResource invalidMemoryResource;
        MemoryResourceScopedSetter invalidMemoryResourceScopedSetter(invalidMemoryResource);

        TestMemoryResource<> memoryResource1("Memory Resource #1");
        TestMemoryResource<> memoryResource2("Memory Resource #2");
        TestMemoryResource<> memoryResource3("Memory Resource #3");
        {
            const allocator_type allocator1(&memoryResource1);
            MainStructure data(allocator1);
            fillData(data, allocator1, hasArray);
            const size_t dataHash = hasArray ? 1549832418 : 4121278947;

            const allocator_type allocator2(&memoryResource2);
            MainStructure equalData(allocator2);
            fillData(equalData, allocator2, hasArray);

            const allocator_type allocator3(&memoryResource3);
            MainStructure diffData(allocator3);
            fillData(diffData, allocator3, hasArray);
            diffData.structField.bit7Array.at(0) = STRUCT_BIT7_ARRAY_ELEMENT0 - 1;
            const size_t diffDataHash = hasArray ? 149684021 : 2721130550;

            zserio::View view(data);
            const size_t viewHash = hasArray ? 4065713639 : 3364006281;
            zserio::View equalView(equalData);
            zserio::View diffView(diffData);
            const size_t diffViewHash = hasArray ? 2665565242 : 1963857884;

            // check that no further memory allocation will take place
            const size_t numAllocations1 = memoryResource1.getNumAllocations();
            const size_t allocatedSize1 = memoryResource1.getAllocatedSize();
            const size_t numAllocations2 = memoryResource2.getNumAllocations();
            const size_t allocatedSize2 = memoryResource2.getAllocatedSize();
            const size_t numAllocations3 = memoryResource3.getNumAllocations();
            const size_t allocatedSize3 = memoryResource3.getAllocatedSize();

            test_utils::hashTest(data, dataHash, equalData, diffData, diffDataHash);
            test_utils::hashTest(view, viewHash, equalView, diffView, diffViewHash);

            ASSERT_EQ(numAllocations1, memoryResource1.getNumAllocations());
            ASSERT_EQ(allocatedSize1, memoryResource1.getAllocatedSize());
            ASSERT_EQ(numAllocations2, memoryResource2.getNumAllocations());
            ASSERT_EQ(allocatedSize2, memoryResource2.getAllocatedSize());
            ASSERT_EQ(numAllocations3, memoryResource3.getNumAllocations());
            ASSERT_EQ(allocatedSize3, memoryResource3.getAllocatedSize());

            // check memory fragmentation in used memory resources
            ASSERT_EQ(0, memoryResource1.getNumDeallocations());
            ASSERT_EQ(0, memoryResource2.getNumDeallocations());
        }

        // check memory leaks in used memory resources
        ASSERT_EQ(memoryResource1.getNumDeallocations(), memoryResource1.getNumAllocations());
        ASSERT_EQ(memoryResource2.getNumDeallocations(), memoryResource2.getNumAllocations());
    }

    void checkWrite(bool hasArray)
    {
        const size_t expectedBitSize =
                (hasArray) ? MAIN_STRUCTURE_WITH_ARRAYS_BIT_SIZE : MAIN_STRUCTURE_WITHOUT_ARRAYS_BIT_SIZE;
        BitBufferType bitBuffer(expectedBitSize);

        // check that default memory resource won't be used
        InvalidMemoryResource invalidMemoryResource;
        MemoryResourceScopedSetter invalidMemoryResourceScopedSetter(invalidMemoryResource);

        TestMemoryResource<> memoryResource("Memory Resource #1");
        {
            const allocator_type allocator(&memoryResource);
            MainStructure data(allocator);
            fillData(data, allocator, hasArray);

            zserio::View view(data);

            // check that no further memory allocation will take place
            const size_t numAllocations = memoryResource.getNumAllocations();
            const size_t allocatedSize = memoryResource.getAllocatedSize();

            zserio::detail::validate(view);
            ASSERT_EQ(expectedBitSize, zserio::detail::bitSizeOf(view));
            zserio::BitStreamWriter writer(bitBuffer);
            zserio::detail::write(writer, view);

            ASSERT_EQ(expectedBitSize, writer.getBitPosition());
            ASSERT_EQ(numAllocations, memoryResource.getNumAllocations());
            ASSERT_EQ(allocatedSize, memoryResource.getAllocatedSize());

            // check memory fragmentation in used memory resource
            ASSERT_EQ(0, memoryResource.getNumDeallocations());
        }

        // check memory leaks in used memory resource
        ASSERT_EQ(memoryResource.getNumDeallocations(), memoryResource.getNumAllocations());
    }

    void checkFuncConstString(bool hasArray)
    {
        // check that default memory resource won't be used
        InvalidMemoryResource invalidMemoryResource;
        MemoryResourceScopedSetter invalidMemoryResourceScopedSetter(invalidMemoryResource);

        TestMemoryResource<> memoryResource("Memory Resource #1");
        {
            const allocator_type allocator(&memoryResource);
            MainStructure data(allocator);
            fillData(data, allocator, hasArray);

            zserio::View view(data);

            // check that no further memory allocation will take place
            const size_t numAllocations = memoryResource.getNumAllocations();
            const size_t allocatedSize = memoryResource.getAllocatedSize();

            ASSERT_EQ("This is constant string longer than 32 bytes!"sv, view.constString());

            ASSERT_EQ(numAllocations, memoryResource.getNumAllocations());
            ASSERT_EQ(allocatedSize, memoryResource.getAllocatedSize());

            // check memory fragmentation in used memory resource
            ASSERT_EQ(0, memoryResource.getNumDeallocations());
        }

        // check memory leaks in used memory resource
        ASSERT_EQ(memoryResource.getNumDeallocations(), memoryResource.getNumAllocations());
    }

    void checkFuncConstCompound(bool hasArray)
    {
        // check that default memory resource won't be used
        InvalidMemoryResource invalidMemoryResource;
        MemoryResourceScopedSetter invalidMemoryResourceScopedSetter(invalidMemoryResource);

        TestMemoryResource<> memoryResource("Memory Resource #1");
        {
            const allocator_type allocator(&memoryResource);
            MainStructure data(allocator);
            fillData(data, allocator, hasArray);

            zserio::View view(data);

            // check that no further memory allocation will take place
            const size_t numAllocations = memoryResource.getNumAllocations();
            const size_t allocatedSize = memoryResource.getAllocatedSize();

            zserio::View expectedConstCompoundView = view.structOptionalField();
            ASSERT_EQ(expectedConstCompoundView, view.constCompound());

            ASSERT_EQ(numAllocations, memoryResource.getNumAllocations());
            ASSERT_EQ(allocatedSize, memoryResource.getAllocatedSize());

            // check memory fragmentation in used memory resource
            ASSERT_EQ(0, memoryResource.getNumDeallocations());
        }

        // check memory leaks in used memory resource
        ASSERT_EQ(memoryResource.getNumDeallocations(), memoryResource.getNumAllocations());
    }

    static constexpr bool ARRAY_IN_UNION_AND_CHOICE = true;
    static constexpr bool COMPOUND_IN_UNION_AND_CHOICE = false;

private:
    bool hasCopyPropagatingAllocator()
    {
        // if allocator is propagating, use invalid memory resource
        InvalidMemoryResource dummyMemoryResource;
        allocator_type dummyAllocator(&dummyMemoryResource);

        return (dummyAllocator ==
                std::allocator_traits<allocator_type>::select_on_container_copy_construction(dummyAllocator));
    }

    static constexpr const char* STRING_FIELD = "String Field Must Be Longer Than 32 Bytes";

    static constexpr uint32_t STRING_ARRAY_SIZE = 3;
    static constexpr const char* STRING_ARRAY_ELEMENT0 = "String Array Element0 Must Be Longer Than 32 Bytes";
    static constexpr const char* STRING_ARRAY_ELEMENT1 = "String Array Element1 Must Be Longer Than 32 Bytes";
    static constexpr const char* STRING_ARRAY_ELEMENT2 = "String Array Element2 Must Be Longer Than 32 Bytes";

    static constexpr uint32_t CHOICE_COMPOUND_ARRAY_SIZE = 2;
    static constexpr uint16_t CHOICE_COMPOUND_ELEMENT0_VALUE16 = 0xAB;
    static constexpr bool CHOICE_COMPOUND_ELEMENT0_IS_VALID = true;
    static constexpr uint16_t CHOICE_COMPOUND_ELEMENT1_VALUE16 = 0xCD;
    static constexpr bool CHOICE_COMPOUND_ELEMENT1_IS_VALID = false;

    static constexpr uint32_t UNION_COMPOUND_ARRAY_SIZE = 1;
    static constexpr uint16_t UNION_COMPOUND_ELEMENT0_VALUE16 = 0xEF;
    static constexpr bool UNION_COMPOUND_ELEMENT0_IS_VALID = true;

    static constexpr uint32_t STRUCT_BIT7_ARRAY_SIZE = 3;
    static constexpr uint8_t STRUCT_BIT7_ARRAY_ELEMENT0 = 0x2B;
    static constexpr uint8_t STRUCT_BIT7_ARRAY_ELEMENT1 = 0x4D;
    static constexpr uint8_t STRUCT_BIT7_ARRAY_ELEMENT2 = 0x6F;
    static constexpr const char* STRUCT_STRING_FIELD = "Structure String Field Must Be Longer Than 32 Bytes";
    static constexpr const char* STRUCT_DEFAULT_STRING_FIELD =
            "Structure Default String Field Must Be Longer Than 32 Bytes";
    static constexpr uint32_t STRUCT_PACKED_UINT16_ARRAY_SIZE = 3;
    static constexpr uint16_t STRUCT_PACKED_UINT16_ARRAY_ELEMENT0 = 0xCAFC;
    static constexpr uint16_t STRUCT_PACKED_UINT16_ARRAY_ELEMENT1 = 0xCAFD;
    static constexpr uint16_t STRUCT_PACKED_UINT16_ARRAY_ELEMENT2 = 0xCAFE;
    static constexpr int8_t STRUCT_PACKED_UINT16_ARRAY_DELTA = 1;
    static constexpr uint8_t STRUCT_PACKED_UINT16_ARRAY_MAX_BIT_NUMBER = 1;
    static constexpr uint32_t STRUCT_PACKED_ELEMENT_ARRAY_SIZE = 3;
    static constexpr uint32_t STRUCT_PACKED_ELEMENT_ARRAY_ELEMENT0_X = 0;
    static constexpr uint32_t STRUCT_PACKED_ELEMENT_ARRAY_ELEMENT0_Y = 10;
    static constexpr uint32_t STRUCT_PACKED_ELEMENT_ARRAY_ELEMENT0_Z = 100;
    static constexpr int8_t STRUCT_PACKED_ELEMENT_ARRAY_DELTA = 1;
    static constexpr uint8_t STRUCT_PACKED_ELEMENT_ARRAY_MAX_BIT_NUMBER = 1;

    static constexpr uint32_t STRUCT_OPTIONAL_NAMES0_SIZE = 2;
    static constexpr const char* STRUCT_OPTIONAL_NAMES0_ELEMENT0 =
            "Optional Name00 Must Be Longer Than 32 Bytes";
    static constexpr const char* STRUCT_OPTIONAL_NAMES0_ELEMENT1 =
            "Optional Name01 Must Be Longer Than 32 Bytes";
    static constexpr uint32_t STRUCT_OPTIONAL_NAMES1_SIZE = 1;
    static constexpr const char* STRUCT_OPTIONAL_NAMES1_ELEMENT0 =
            "Optional Name10 Must Be Longer Than 32 Bytes";

    static constexpr uint8_t EXTERNAL_FIELD_VAR_SIZE = 11;
    static constexpr uint16_t EXTERNAL_FIELD_DATA = 0xABE0;

    static constexpr uint32_t EXTERNAL_ARRAY_SIZE = 2;
    static constexpr uint8_t EXTERNAL_ARRAY_ELEMENT0_VAR_SIZE = 7;
    static constexpr uint8_t EXTERNAL_ARRAY_ELEMENT0_DATA = 0xAE;
    static constexpr uint8_t EXTERNAL_ARRAY_ELEMENT1_VAR_SIZE = 15;
    static constexpr uint16_t EXTERNAL_ARRAY_ELEMENT1_DATA = 0xEA;

    static constexpr uint8_t BYTES_FIELD_VAR_SIZE = 2;
    static constexpr uint16_t BYTES_FIELD_DATA = 0xC0DE;

    static constexpr uint32_t BYTES_ARRAY_SIZE = 2;
    static constexpr uint8_t BYTES_ARRAY_ELEMENT0_VAR_SIZE = 2;
    static constexpr uint16_t BYTES_ARRAY_ELEMENT0_DATA = 0xCAFE;
    static constexpr uint8_t BYTES_ARRAY_ELEMENT1_VAR_SIZE = 1;
    static constexpr uint8_t BYTES_ARRAY_ELEMENT1_DATA = 0xC0;

    static constexpr size_t MAIN_STRUCTURE_WITH_ARRAYS_BIT_SIZE = 3978;
    static constexpr size_t MAIN_STRUCTURE_WITHOUT_ARRAYS_BIT_SIZE = 3945;
};

TEST_F(ComplexAllocationTest, read)
{
    checkRead(ARRAY_IN_UNION_AND_CHOICE);
    checkRead(COMPOUND_IN_UNION_AND_CHOICE);
}

TEST_F(ComplexAllocationTest, copyConstructor)
{
    checkCopyConstructor(ARRAY_IN_UNION_AND_CHOICE);
    checkCopyConstructor(COMPOUND_IN_UNION_AND_CHOICE);
}

TEST_F(ComplexAllocationTest, moveConstructor)
{
    checkMoveConstructor(ARRAY_IN_UNION_AND_CHOICE);
    checkMoveConstructor(COMPOUND_IN_UNION_AND_CHOICE);
}

TEST_F(ComplexAllocationTest, copyAssignmentOperator)
{
    checkCopyAssignmentOperator(ARRAY_IN_UNION_AND_CHOICE);
    checkCopyAssignmentOperator(COMPOUND_IN_UNION_AND_CHOICE);
}

TEST_F(ComplexAllocationTest, moveAssignmentOperator)
{
    checkMoveAssignmentOperator(ARRAY_IN_UNION_AND_CHOICE);
    checkMoveAssignmentOperator(COMPOUND_IN_UNION_AND_CHOICE);
}

TEST_F(ComplexAllocationTest, allocatorConstructor)
{
    checkAllocatorConstructor(ARRAY_IN_UNION_AND_CHOICE);
    checkAllocatorConstructor(COMPOUND_IN_UNION_AND_CHOICE);
}

TEST_F(ComplexAllocationTest, fieldConstructor)
{
    checkFieldConstructor(ARRAY_IN_UNION_AND_CHOICE);
    checkFieldConstructor(COMPOUND_IN_UNION_AND_CHOICE);
}

TEST_F(ComplexAllocationTest, bitSizeOf)
{
    checkBitSizeOf(ARRAY_IN_UNION_AND_CHOICE);
    checkBitSizeOf(COMPOUND_IN_UNION_AND_CHOICE);
}

TEST_F(ComplexAllocationTest, comparisonOperators)
{
    checkComparisonOperators(ARRAY_IN_UNION_AND_CHOICE);
    checkComparisonOperators(COMPOUND_IN_UNION_AND_CHOICE);
}

TEST_F(ComplexAllocationTest, write)
{
    checkWrite(ARRAY_IN_UNION_AND_CHOICE);
    checkWrite(COMPOUND_IN_UNION_AND_CHOICE);
}

TEST_F(ComplexAllocationTest, funcConstString)
{
    checkFuncConstString(ARRAY_IN_UNION_AND_CHOICE);
    checkFuncConstString(COMPOUND_IN_UNION_AND_CHOICE);
}

TEST_F(ComplexAllocationTest, funcConstCompound)
{
    checkFuncConstCompound(ARRAY_IN_UNION_AND_CHOICE);
    checkFuncConstCompound(COMPOUND_IN_UNION_AND_CHOICE);
}

TEST_F(ComplexAllocationTest, stdHash)
{
    checkStdHash(ARRAY_IN_UNION_AND_CHOICE);
    checkStdHash(COMPOUND_IN_UNION_AND_CHOICE);
}

} // namespace complex_allocation
