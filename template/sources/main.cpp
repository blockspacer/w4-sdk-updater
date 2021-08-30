#include "W4Framework.h"

W4_USE_UNSTRICT_INTERFACE

struct emptyApp : public IGame
{
    void onStart() override
    {
        gui::createWidget<Label>(nullptr, "OLOLO, ITS ONLY STUB", ivec2(540, 1800));
    }

    void onUpdate(float dt) override
    {
	// write your code here...		

    }
private:

};

W4_RUN(emptyApp)
