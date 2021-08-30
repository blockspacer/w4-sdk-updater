template<typename T, typename...Args>
sptr<Collider> Collider::create(Node* parent, Args&&... args)
{
    auto collider = sptr<Collider>(new Collider(parent));
    auto volume = w4::make::sptr<T>(std::forward<Args>(args)...);
    collider->setVolume(volume);
    return collider;
}