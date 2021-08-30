class MaterialInst;
class MaterialParameterPackImplBase
{
public:
    explicit MaterialParameterPackImplBase(MaterialInst& mat);

protected:
    const MaterialInst& getMaterialInst() const;
    MaterialInst& getMaterialInst();

private:
    const size_t m_objOffset;
};

template<typename T>
class MaterialParameterPackImpl: public MaterialParameterPackImplBase
{
public:
    using MaterialParameterPackImplBase::MaterialParameterPackImplBase;

    void _setParamImpl(const Material::ParamIDType& id,  T value);
    T _getParamImpl(const Material::ParamIDType& paramId) const;
    static bool getParamTyped(const sptr<Material>& mat, const std::string& name, Material::ParamIDType& paramId);
};

template <typename T>
struct isOuterManagedSPtr
{
    template <typename C = T>
    static auto test(int) -> decltype(std::static_pointer_cast<core::IOuterManaged>(std::declval<C>()), std::true_type());
    static auto test(...) -> std::false_type;
public:
    enum { value = decltype(test(0))::value };
};

template <typename T>
using EnableIfIsOuterManagedSptr = typename std::enable_if<isOuterManagedSPtr<T>::value>::type;

template<typename T, typename = void>
class MaterialParameterPackImplTyped: public MaterialParameterPackImpl<T>
{
    using Base = MaterialParameterPackImpl<T>;
public:
    using MaterialParameterPackImpl<T>::MaterialParameterPackImpl;
    void _setParam(const std::string& name, T value);
    bool _checkParam(const std::string& name) const;
    T _getParam(const std::string& name) const;
    void _saveParams(std::unordered_map<std::string, ParamData>& res) const;

private:
#if !defined(__EMSCRIPTEN__) && defined(W4_ENABLE_SERIALIZERS)
    std::unordered_map<std::string, T> m_values;
#endif
};

template<typename T>
class MaterialParameterPackImplTyped<T, EnableIfIsOuterManagedSptr<T>> : private MaterialParameterPackImplTyped<core::OuterID::ndxType>
{
    using InnerType = core::OuterID::ndxType;
    using MaterialParameterPackImplTyped<InnerType>::MaterialParameterPackImplTyped;

public:
    void _setParam(const std::string& name, T value);
    bool _checkParam(const std::string& name) const;
    T    _getParam(const std::string& name) const;
    void _saveParams(std::unordered_map<std::string, ParamData>& res) const;

private:
#if !defined(__EMSCRIPTEN__) && defined(W4_ENABLE_SERIALIZERS)
    std::unordered_map<std::string, T> m_values;
#else
    using ParamsValues = std::unordered_map<core::OuterID::ndxType , T>;
    ParamsValues m_values;
#endif
};

enum class TextureId
{
    TEXTURE_0 = 0,
    TEXTURE_1,
    TEXTURE_2,
    TEXTURE_3,
    TEXTURE_4,
    TEXTURE_5,
    SIZE
};

enum class CubemapId
{
    CUBEMAP_0 = 0,
    CUBEMAP_1,
    SIZE
};

template<typename T>
class MaterialParameterPack : public MaterialParameterPackImplTyped<T>
{
public:
    using MaterialParameterPackImplTyped<T>::MaterialParameterPackImplTyped;
};

template<>
class MaterialParameterPack<sptr<resources::Texture>> : public MaterialParameterPackImplTyped<sptr<resources::Texture>>
{
public:
using MaterialParameterPackImplTyped<sptr<resources::Texture>>::MaterialParameterPackImplTyped;

void setTexture(TextureId id, const sptr<resources::Texture>& ptr);
sptr<resources::Texture> getTexture(TextureId id);
bool hasTexture(TextureId id);

private:
static std::unordered_map<TextureId, std::string> m_textureIdMapping;
};

template<>
class MaterialParameterPack<sptr<resources::Cubemap>> : public MaterialParameterPackImplTyped<sptr<resources::Cubemap>>
{
public:
using MaterialParameterPackImplTyped<sptr<resources::Cubemap>>::MaterialParameterPackImplTyped;

void setCubemap(CubemapId id, const sptr<resources::Cubemap>& ptr);
sptr<resources::Cubemap> getCubemap(CubemapId id);
bool hasCubemap(CubemapId id);
private:
static std::unordered_map<CubemapId, std::string> m_cubemapIdMapping;
};

template<typename ... Args>
class MaterialParameters: public MaterialParameterPack<Args>...
{
public:
    explicit MaterialParameters(MaterialInst& mat);

    template<typename T>
    T getParam(const std::string& name);

    template<typename T>
    void setParam(const std::string& name, const T& val);

    void _saveParams(std::unordered_map<std::string, ParamData>& res) const;
};