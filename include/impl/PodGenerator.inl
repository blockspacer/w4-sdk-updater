

template <typename T>
struct isPodControlType
{
    template <typename C = T>
    static auto test(int) -> decltype(C::update(), std::true_type());
    static auto test(...) -> std::false_type;
public:
    enum { value = decltype(test(0))::value };
};

#define __FieldPartSingle(r, data, fieldTuple)                                                              \
    BOOST_PP_TUPLE_ELEM(0, fieldTuple) BOOST_PP_TUPLE_ELEM(1, fieldTuple) = BOOST_PP_TUPLE_ELEM(0, fieldTuple)()

#define __LIST_VALS(z, n, val) BOOST_PP_COMMA_IF(n) val

#define __FieldPartArray(r, data, fieldTuple)                                                               \
    BOOST_PP_TUPLE_ELEM(0, fieldTuple) BOOST_PP_TUPLE_ELEM(1, fieldTuple) [BOOST_PP_TUPLE_ELEM(2, fieldTuple)] \
        = { BOOST_PP_REPEAT(BOOST_PP_TUPLE_ELEM(2, fieldTuple), __LIST_VALS, BOOST_PP_TUPLE_ELEM(0, fieldTuple)()) }

#define __FieldIsArray(fieldTuple)  \
    BOOST_PP_GREATER(BOOST_PP_TUPLE_ELEM(2, fieldTuple), 0)

#define __FieldIsControl(fieldTuple)  \
    BOOST_PP_GREATER(BOOST_PP_TUPLE_ELEM(3, fieldTuple), 0)

#define __FieldPart(r, data, fieldTuple)                                                                    \
    BOOST_PP_IF(                                                                                            \
            __FieldIsArray(fieldTuple),                                                                     \
            __FieldPartArray,                                                                               \
            __FieldPartSingle                                                                               \
    )(r, data, fieldTuple);

#define __ControlPartImpl(r, TypeName, fieldTuple)                                                          \
    fieldDescr.control = static_cast<int>(TypeName::__ControllEnum::BOOST_PP_TUPLE_ELEM(1, fieldTuple));

#define __ControlPartDummy(r, data, fieldTuple)                                                             \
    fieldDescr.control = false;

#define __FieldDescrTypePartImpl(r, TypeName, fieldTuple)                                                   \
    fieldDescr.type = BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(0, fieldTuple));                               \
    fieldDescr.offset = (int)offsetof(struct TypeName, BOOST_PP_TUPLE_ELEM(1, fieldTuple));                 \
    fieldDescr.size  = sizeof(TypeName::BOOST_PP_TUPLE_ELEM(1, fieldTuple)                                  \
            BOOST_PP_IF(                                                                                    \
                    __FieldIsArray(fieldTuple),                                                             \
                    [0],                                                                                    \
                    BOOST_PP_EMPTY()                                                                        \
            )                                                                                               \
    );                                                                                                      \
    fieldDescr.count = BOOST_PP_TUPLE_ELEM(2, fieldTuple);                                                  \
    BOOST_PP_IF(                                                                                            \
            __FieldIsControl(fieldTuple),                                                                   \
            __ControlPartImpl ,                                                                            \
            __ControlPartDummy                                                                             \
    )(r, TypeName, fieldTuple)


#define __FieldDescrTypePart(r, TypeName, fieldTuple)                                                       \
{                                                                                                           \
    w4::core::PodTypeDescr fieldDescr;                                                                      \
    __FieldDescrTypePartImpl(r, TypeName, fieldTuple)                                                       \
    descr[ BOOST_PP_STRINGIZE(BOOST_PP_TUPLE_ELEM(1, fieldTuple)) ] = fieldDescr;                           \
}

#define __N_Control_Fields_PRED(d, data) BOOST_PP_TUPLE_ELEM(3, 1, data)

#define __N_Control_Fields_OP(d, data)                                                                      \
   __N_Control_Fields_OP_Impl(                                                                              \
      d,                                                                                                    \
      BOOST_PP_TUPLE_ELEM(3, 0, data),                                                                      \
      BOOST_PP_TUPLE_ELEM(3, 1, data),                                                                      \
      BOOST_PP_TUPLE_ELEM(3, 2, data)                                                                       \
   )

#define __N_Control_Fields_OP_Impl(d, res, i, Fields)                                                       \
   (                                                                                                        \
      BOOST_PP_ADD_D(                                                                                       \
         d, res,                                                                                            \
         BOOST_PP_TUPLE_ELEM(3, BOOST_PP_SEQ_ELEM(BOOST_PP_DEC(i), Fields))),                               \
      BOOST_PP_DEC(i),                                                                                      \
      Fields                                                                                                \
   )

#define __N_Control_Fields(Fields)                                                                          \
   BOOST_PP_TUPLE_ELEM(                                                                                     \
      0,                                                                                                    \
      BOOST_PP_WHILE(                                                                                       \
         __N_Control_Fields_PRED,                                                                           \
         __N_Control_Fields_OP,                                                                             \
         (0, BOOST_PP_SEQ_SIZE(Fields), Fields)                                                             \
      )                                                                                                     \
   )

#define __ControllEnumPartImpl(r, data, fieldTuple)                                                         \
    BOOST_PP_TUPLE_ELEM(1, fieldTuple),

#define __ControllEnumPartDummy(r, data, fieldTuple)

#define __ControllEnumPart(r, data, fieldTuple)                                                             \
    BOOST_PP_IF(                                                                                            \
            __FieldIsControl(fieldTuple),                                                                   \
            __ControllEnumPartImpl ,                                                                        \
            __ControllEnumPartDummy                                                                         \
    )(r, data, fieldTuple)

#define __UpdateEnumPart(r, data, fieldTuple)                                                               \
    __processField(BOOST_PP_TUPLE_ELEM(1, fieldTuple));

#define __Controll_Impl(Fields)                                                                             \
    enum class __ControllEnum                                                                               \
    {                                                                                                       \
        BOOST_PP_SEQ_FOR_EACH(__ControllEnumPart, _, Fields)                                                \
    };                                                                                                      \
    bool __control[__N_Control_Fields(Fields)]                                                              \
            = { BOOST_PP_REPEAT(__N_Control_Fields(Fields), __LIST_VALS, false) };                          \
    std::function<void (__ControllEnum)> _onFieldChanged;                                                   \
                                                                                                            \
    template<typename T>                                                                                    \
    void __processField(T& value, typename std::enable_if<isPodControlType<T>::value>::type* = 0)           \
    {                                                                                                       \
        value.__processOnChange();                                                                          \
    }                                                                                                       \
    template<typename T>                                                                                    \
    void __processField(T& value, typename std::enable_if<!isPodControlType<T>::value>::type* = 0)          \
    {}                                                                                                      \
                                                                                                            \
    void __processOnChange()                                                                                \
    {                                                                                                       \
        if (_onFieldChanged)                                                                                \
        {                                                                                                   \
            for (int i = 0; i < __N_Control_Fields(Fields); ++i)                                            \
            {                                                                                               \
                if (__control[i])                                                                           \
                {                                                                                           \
                    _onFieldChanged(static_cast<__ControllEnum>(i));                                        \
                    __control[i] = false;                                                                   \
                }                                                                                           \
            }                                                                                               \
        }                                                                                                   \
        BOOST_PP_SEQ_FOR_EACH(__UpdateEnumPart, _, Fields)                                                  \
    }                                                                                                       \

#define __Controll_ImplDummy(Fields)

#define __Controll_Struct(Fields)                                                                           \
    BOOST_PP_IF(__N_Control_Fields(Fields),                                                                 \
        __Controll_Impl ,                                                                                   \
        __Controll_ImplDummy                                                                                \
    )(Fields)

#define POD_STRUCT(Type, Fields, ...)                                                                       \
struct Type                                                                                                 \
{                                                                                                           \
    __Controll_Struct(Fields);                                                                              \
    BOOST_PP_SEQ_FOR_EACH(__FieldPart, _, Fields)                                                           \
    static inline const w4::core::PodFieldsDescr getTypeDescr()                                             \
    {                                                                                                       \
        static const w4::core::PodFieldsDescr descr = []() -> w4::core::PodFieldsDescr                      \
        {                                                                                                   \
            w4::core::PodFieldsDescr descr;                                                                 \
            BOOST_PP_SEQ_FOR_EACH(__FieldDescrTypePart, Type, Fields)                                       \
            return descr;                                                                                   \
        }();                                                                                                \
        return descr;                                                                                       \
    }                                                                                                       \
    static w4::core::OuterID::ndxType getTypeId()                                                           \
    {                                                                                                       \
        static w4::core::OuterID::ndxType const res = w4::core::PodTypeIdRegistry::registerPodType(BOOST_PP_STRINGIZE(Type), getTypeDescr());                \
        return res;                                                                                         \
    }                                                                                                       \
__VA_ARGS__                                                                                                 \
};                                                                                                          \
struct Type##_Holder : Type, public w4::core::IOuterManaged                                                 \
{                                                                                                           \
    Type##_Holder()                                                                                         \
    {                                                                                                       \
        m_ID.set(w4::core::detail::registerPodValue(static_cast<Type*>(this), getTypeId()));                \
    }                                                                                                       \
    void outerCreator() override {}                                                                         \
    void outerDeleter() override {}                                                                         \
};                                                                                                          \
namespace w4::make{                                                                                         \
    inline w4::sptr<Type##_Holder> pod_##Type()                                                             \
    {                                                                                                       \
        return w4::make::sptr<Type##_Holder>();                                                             \
    }                                                                                                       \
}

#define POD_FIELD(Type, Name)                                                                               \
    ((Type, Name, 0, 0))

#define POD_ARRAY(Type, Name, Length)                                                                       \
    ((Type, Name, Length, 0))

#define POD_CONTROL_FIELD(Type, Name)                                                                       \
    ((Type, Name, 0, 1))

#define POD_CONTROL_ARRAY(Type, Name, Length)                                                               \
    ((Type, Name, Length, 1))
