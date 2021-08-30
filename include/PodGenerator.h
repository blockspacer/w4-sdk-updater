#pragma once

#include <type_traits>
#include <unordered_map>

#include "external/boost/preprocessor/seq/for_each.hpp"
#include "external/boost/preprocessor/tuple/rem.hpp"
#include "external/boost/preprocessor/tuple/elem.hpp"
#include "external/boost/preprocessor/tuple/size.hpp"
#include "external/boost/preprocessor/control/if.hpp"
#include "external/boost/preprocessor/logical/and.hpp"
#include "external/boost/preprocessor/comparison/greater.hpp"
#include "external/boost/preprocessor/punctuation/comma_if.hpp"
#include "external/boost/preprocessor/repetition/repeat.hpp"
#include "external/boost/preprocessor/stringize.hpp"
#include "external/boost/preprocessor/empty.hpp"
#include "external/boost/preprocessor/arithmetic/add.hpp"
#include "external/boost/preprocessor/arithmetic/dec.hpp"
#include "external/boost/preprocessor/seq/size.hpp"
#include "external/boost/preprocessor/seq/elem.hpp"
#include "external/boost/preprocessor/control/while.hpp"

#include "IOuterID.h"

namespace w4::core
{
    struct PodTypeDescr
    {
        const char* type;
        uint16_t offset;
        uint16_t size;
        uint16_t count;
        bool control;
    };

    using PodFieldsDescr = std::unordered_map<std::string, PodTypeDescr>;

    namespace detail {
        OuterID::ndxType registerPodValue(void *value, OuterID::ndxType typeID);
        OuterID::ndxType registerPodType(const std::string& name, const PodFieldsDescr& descr);
        size_t getPodTypeSize(OuterID::ndxType typeID);
    }

    struct PodTypeIdRegistry
    {
        static OuterID::ndxType getPodTypeID(const std::string& name);
        static OuterID::ndxType registerPodType(const std::string& name, const PodFieldsDescr& descr);
    private:
        static std::unordered_map<std::string, OuterID::ndxType> m_nameToId;
    };
}

#include "impl/PodGenerator.inl"

using utf8CharPtr = char *;
