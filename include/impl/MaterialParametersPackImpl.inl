template<typename T>
void MaterialParameterPackImpl<T>::_setParamImpl(const Material::ParamIDType& paramId, T value)
{
    W4_ASSERT( paramId.size == sizeof(T));
    *reinterpret_cast<T*>(getMaterialInst().getParamsRawData() + paramId.offset) = value;
}

template<typename T>
T MaterialParameterPackImpl<T>::_getParamImpl(const Material::ParamIDType& paramId) const
{
    W4_ASSERT( paramId.size == sizeof(T));
    return *reinterpret_cast<const T*>(getMaterialInst().getParamsRawData() + paramId.offset);
}

template <typename T>
bool MaterialParameterPackImpl<T>::getParamTyped(const sptr<Material>& mat, const std::string& name, Material::ParamIDType& paramId)
{
    if (!mat->getParamId(name, paramId))
    {
        return false;
    }
    return paramId.type == ParamTypeMapping<T>::type();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T, typename C>
void MaterialParameterPackImplTyped<T, C>::_setParam(const std::string& name, T value)
{
#if !defined(__EMSCRIPTEN__) && defined(W4_ENABLE_SERIALIZERS)
    m_values[name] = value;
#else
    Material::ParamIDType paramId;
    if (!Base::getParamTyped(Base::getMaterialInst().getMaterial(), name, paramId))
    {
        FATAL_ERROR("Material: set param: invalid param %s", name.c_str());
    }
    Base::_setParamImpl(paramId, value);
#endif
}

template<typename T, typename C>
T MaterialParameterPackImplTyped<T, C>::_getParam(const std::string& name) const
{
#if !defined(__EMSCRIPTEN__) && defined(W4_ENABLE_SERIALIZERS)
    if (auto it = m_values.find(name); it != m_values.end())
    {
        return it->second;
    }
    FATAL_ERROR("Material: get param: invalid param %s", name.c_str());
    return T();
#else
    Material::ParamIDType paramId;
    if (!Base::getParamTyped(Base::getMaterialInst().getMaterial(), name, paramId))
    {
        FATAL_ERROR("Material: get param: invalid param %s", name.c_str());
    }

    return Base::_getParamImpl(paramId);
#endif
}

template<typename T, typename C>
bool MaterialParameterPackImplTyped<T, C>::_checkParam(const std::string& name) const
{
#if !defined(__EMSCRIPTEN__) && defined(W4_ENABLE_SERIALIZERS)
    return true;
#else
    Material::ParamIDType paramId;
    return getParamTyped(Base::getMaterialInst().getMaterial(), name, paramId);
#endif
}

template<typename T, typename C>
void MaterialParameterPackImplTyped<T, C>::_saveParams(std::unordered_map<std::string, ParamData>& res) const
{
#if !defined(__EMSCRIPTEN__) && defined(W4_ENABLE_SERIALIZERS)
    for (auto & [n, v]: m_values)
    {
        res[n] = ParamData(v);
    }
#else
    cref<Material> mat = Base::getMaterialInst().getMaterial();
    mat->foreachParam(ParamTypeMapping<T>::type(), [&](const std::string& name)
    {
        res[name] = ParamData(_getParam(name));
    });
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>
void MaterialParameterPackImplTyped<T, EnableIfIsOuterManagedSptr<T>>::_setParam(const std::string& name, T value)
{
#if !defined(__EMSCRIPTEN__) && defined(W4_ENABLE_SERIALIZERS)
m_values[name] = value;
#else
Material::ParamIDType paramId;
if (!MaterialParameterPackImpl<T>::getParamTyped(getMaterialInst().getMaterial(), name, paramId))
{
FATAL_ERROR("Material: set param: invalid param %s", name.c_str());
}

MaterialParameterPackImplTyped<InnerType>::_setParamImpl(paramId, value->getInternal());
m_values[paramId.id] = value;
#endif
}

template<typename T>
T MaterialParameterPackImplTyped<T, EnableIfIsOuterManagedSptr<T>>::_getParam(const std::string& name) const
{
#if !defined(__EMSCRIPTEN__) && defined(W4_ENABLE_SERIALIZERS)
if (auto it = m_values.find(name); it != m_values.end())
    {
        return it->second;
    }
    return nullptr;
#else
Material::ParamIDType paramId;
if (!MaterialParameterPackImpl<T>::getParamTyped(getMaterialInst().getMaterial(), name, paramId))
{
FATAL_ERROR("Material: get param: invalid param %s", name.c_str());
}
if (auto it = m_values.find(paramId.id); it != m_values.end())
{
return it->second;
}
return nullptr;
#endif
}

template<typename T>
bool MaterialParameterPackImplTyped<T, EnableIfIsOuterManagedSptr<T>>::_checkParam(const std::string& name) const
{
#if !defined(__EMSCRIPTEN__) && defined(W4_ENABLE_SERIALIZERS)
return true;
#else
Material::ParamIDType paramId;
return MaterialParameterPackImpl<T>::getParamTyped(getMaterialInst().getMaterial(), name, paramId);
#endif
}

template<typename T>
void MaterialParameterPackImplTyped<T, EnableIfIsOuterManagedSptr<T>>::_saveParams(std::unordered_map<std::string, ParamData>& res) const
{
#if !defined(__EMSCRIPTEN__) && defined(W4_ENABLE_SERIALIZERS)
for (auto &[n, v]: m_values)
{
        res[n] = ParamData(v);
}
#else
auto & mat = *getMaterialInst().getMaterial();
for (auto & p: m_values)
{
auto name = mat.getParamName(p.first);
res[name] = ParamData(p.second);
}
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename ... Args>
MaterialParameters<Args...>::MaterialParameters(MaterialInst& mat)
        : MaterialParameterPack<Args>(mat)...
{
}

template<typename ... Args> template<typename T>
T MaterialParameters<Args...>::getParam(const std::string& name)
{
    return static_cast<MaterialParameterPack<T>*>(this)->_getParam(name);
}

template<typename ... Args>
template<typename T>
void MaterialParameters<Args...>::setParam(const std::string& name, const T& val)
{
    if constexpr (std::is_same_v<T, w4::math::color>)
    {
        static_cast<MaterialParameterPack<w4::math::vec4> *>(this)->_setParam(name, static_cast<const w4::math::vec4&>(val));
    }
    else
    {
        static_cast<MaterialParameterPack <T> *>(this)->_setParam(name, val);
    }
}

template<typename ... Args>
void MaterialParameters<Args...>::_saveParams(std::unordered_map<std::string, ParamData>& res) const
{
    (static_cast<const MaterialParameterPack<Args>*>(this)->_saveParams(res), ...);
}