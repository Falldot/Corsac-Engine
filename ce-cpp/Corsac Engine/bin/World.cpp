#include "World.h"

Manager manager;

auto& player(manager.addEntity());
auto& label(manager.addEntity());

void World::Init()
{
    CRSC_LoadTexture("terrain", "./bin/data/sprites/mapTites.png");
    CRSC_LoadTexture("player", "./bin/data/sprites/fox.png");
    CRSC_LoadTexture("projectile", "./bin/data/sprites/bolt.png");

    CRSC_LoadFont("main", "./bin/data/Determination2.ttf", 24);

    Location = new CRSC_Map("terrain", 3, 36);
    Location->Load("./bin/data/maps/Map.map", 36, 36);

    player.addComponent<Transform>(1000, 600, 32, 32, 2);
    player.addComponent<Sprite>("player", true);
    player.getComponent<Sprite>().addAnimation("Idle", 0, 6, 300);
    player.getComponent<Sprite>().addAnimation("Right", 1, 6, 100);
    player.getComponent<Sprite>().addAnimation("Down", 2, 6, 100);
    player.getComponent<Sprite>().addAnimation("Up", 3, 6, 100);
    player.getComponent<Sprite>().addAnimation("DownRight", 4, 6, 100);
    player.addComponent<KeyboardController>();
    player.addComponent<FixCamera>(1200, 1200);
    player.addComponent<Collider>("player");
    player.addComponent<ClickUp>();
    player.addComponent<ClickDown>();
    player.addComponent<Hover>();

    player.addGroup(ClickUps);
    player.addGroup(ClickDowns);
    player.addGroup(Hovers);

    player.addGroup(Players);

    label.addComponent<UILabel>(10, 10, "Test String", "main", 255, 255, 0, 255);

    //assets->CreateProjectile(Vector2D(600, 600), Vector2D(1, 0), 200, 2, "projectile");
    //assets->CreateProjectile(Vector2D(600, 620), Vector2D(1, 0), 200, 2, "projectile");
    //assets->CreateProjectile(Vector2D(400, 600), Vector2D(1, 1), 200, 2, "projectile");
    //assets->CreateProjectile(Vector2D(600, 600), Vector2D(1, -1), 200, 2, "projectile");

    run = true;
}

auto& tiles(manager.getGroup(CRSC_Scene::Map));
auto& players(manager.getGroup(CRSC_Scene::Players));
auto& colliders(manager.getGroup(CRSC_Scene::Colliders));
auto& projectiles(manager.getGroup(CRSC_Scene::Projectiles));

auto& clickUps(manager.getGroup(CRSC_Scene::ClickUps));
auto& clickDowns(manager.getGroup(CRSC_Scene::ClickDowns));
auto& hovers(manager.getGroup(CRSC_Scene::Hovers));

void World::Event()
{
    SDL_PollEvent(&event);

    switch (event.type) {
    case SDL_QUIT:
        run = false;
        break;
    case SDL_MOUSEBUTTONUP:
        for (auto& c : clickUps)
        {
            if (CRSC_MouseAABB(c->getComponent<Collider>().collider)) c->getComponent<ClickUp>().setEvent(true);
        }
        break;
    case SDL_MOUSEBUTTONDOWN:
        for (auto& c : clickDowns)
        {
            if (CRSC_MouseAABB(c->getComponent<Collider>().collider)) c->getComponent<ClickDown>().setEvent(true);
        }
        break;
    case SDL_MOUSEMOTION:
        for (auto& c : hovers)
        {
            if (CRSC_MouseAABB(c->getComponent<Collider>().collider)) c->getComponent<Hover>().setEvent(true);
        }
        break;
    default:
        break;
    }
    const Uint8* keystate = SDL_GetKeyboardState(NULL);
    if (keystate[SDL_SCANCODE_ESCAPE]) run = false;
}
void World::Update(float fps)
{
    label.getComponent<UILabel>().SetLabelText(std::to_string((int)fps), "main");

    SDL_Rect playerCol = player.getComponent<Collider>().collider;
    bool count = true;
    for (auto& c : colliders)
    {
        SDL_Rect cCol = c->getComponent<Collider>().collider;
        if (CRSC_AABB(cCol, playerCol))
        {
            player.getComponent<Transform>().position = playerPos;
            count = false;
        }
    }
    if (count) {
        playerPos = player.getComponent<Transform>().position;
    }


    if (player.getComponent<ClickUp>().getEvent()) {
        player.getComponent<Sprite>().setColor(255, 0, 0);
        player.getComponent<ClickUp>().setEvent(false);
    }
    if (player.getComponent<ClickDown>().getEvent()) {
        player.getComponent<Sprite>().setColor(0, 255, 0);
        player.getComponent<ClickDown>().setEvent(false);
    }

    camera = player.getComponent<FixCamera>().camera;

    for (auto& p : projectiles)
    {
        if (CRSC_AABB(player.getComponent<Collider>().collider, p->getComponent<Collider>().collider))
        {
            p->Destroy();
        }
    }

    manager.Update();
    manager.Refresh();
}
void World::Render()
{
    SDL_RenderClear(CRSC_App::Renderer);

    for (auto& t : tiles)
    {
        t->Render();
    }
    for (auto& c : colliders)
    {
        c->Render();
    }
    for (auto& p : players)
    {
        p->Render();
    }
    for (auto& p : projectiles)
    {
        p->Render();
    }

    label.Render();

    SDL_RenderPresent(CRSC_App::Renderer);
}
void World::Destroy()
{
    for (auto& t : tiles)
    {
        t->Destroy();
    }
    for (auto& c : colliders)
    {
        c->Destroy();
    }
    for (auto& p : players)
    {
        p->Destroy();
    }
    for (auto& p : projectiles)
    {
        p->Destroy();
    }
    label.Destroy();

    CRSC_DestroyAllTexture();
    CRSC_DestroyAllFonts();

    manager.Refresh();

}