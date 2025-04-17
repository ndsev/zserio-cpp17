#!/bin/sh
java -jar ../../distr/zserio.jar -src test_object test_object.zs -cpp17 . -withTypeInfoCode -setCppAllocator std -setTopLevelPackage test_object.std_allocator
java -jar ../../distr/zserio.jar -src test_object test_object.zs -cpp17 . -withTypeInfoCode -setCppAllocator std-polymorphic -setTopLevelPackage test_object.std_polymorphic_allocator
java -jar ../../distr/zserio.jar -src test_object test_object.zs -cpp17 . -withTypeInfoCode -setCppAllocator polymorphic -setTopLevelPackage test_object.polymorphic_allocator
