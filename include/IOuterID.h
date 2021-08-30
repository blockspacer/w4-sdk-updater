#pragma once

#include "W4Common.h"

namespace w4::core {

struct OuterID
{
    using ndxType = size_t;
    static constexpr size_t invalid_value = 0;

    explicit OuterID(ndxType = invalid_value);
   ~OuterID();

    [[nodiscard]] ndxType get()   const { return m_ndx;}
    void set(ndxType ndx) { m_ndx = ndx; }
    void clean() { m_ndx = invalid_value;}
    operator bool() const { return m_ndx != invalid_value;}

private:
    ndxType m_ndx = invalid_value;

};

struct IOuterManaged
{
    IOuterManaged() = default;
    virtual ~IOuterManaged() = default;

    [[nodiscard]] virtual bool     isValid()     const { return m_ID; }
    [[nodiscard]] OuterID::ndxType getInternal() const { return m_ID.get(); }

protected:
    virtual void outerCreator() = 0;
    virtual void outerDeleter() = 0;

    OuterID m_ID;
};




} //namespace
