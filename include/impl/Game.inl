
template<typename Impl>
void Game::run(const char* parentObject)
{
    auto initInstance = []()
    {
        m_instance->onStart();
        m_instance->initUpdate();

        if(Config.UseSimpleInput)
        {
            event::Keyboard::Down::subscribe(std::bind(&IGame::onKey, m_instance->m_impl, std::placeholders::_1));
            event::Touch::Begin::subscribe(std::bind(&IGame::onTouch, m_instance->m_impl, std::placeholders::_1));
        }
    };

    if(!m_instance)
    {
        using namespace std::placeholders;

        m_instance = sptr<Game>(new Game(sptr<IGame>(new Impl()), parentObject));

        event::Resize::subscribe([](auto& evt)
        {
            m_instance->m_impl->onResize(evt.size);
            Render::resize(evt.size);
        });

        utils::InitW4Lib();
        core::Timer::init();
        m_instance->onConfig();
        Render::init(Config.RSettings);

        Config.log();

        if (Config.UseDefaultRenderPass)
        {
            W4_LOG_INFO("Using default render pass, create default root node ...");
            auto id = Render::addPass();
            Render::getPass(id)->setEnabled(true);
            Render::getPass(id)->setRoot(make::sptr<render::RootNode>());
        }

        event::Resize::fire(platform::Platform::getSize());
        event::Event::performEmitAll();
        platform::Platform::showLoader();

        if(Config.VFSInit)
        {
            W4_LOG_INFO("Init VFS ...");
            auto& vfs = filesystem::VFS::instance();

            auto vfsInit = [&]
            {
                auto vfsInitCb = [&]
                {
                    initInstance();
                };
                vfs.init(vfsInitCb);
            };

            Config.VFSClean ? vfs.clean(vfsInit) : vfsInit();
        }
        else
        {
            initInstance();
        }
    }
}
