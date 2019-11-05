#ifndef CAMERA_H
#define CAMERA_H

struct Camera
{
    float invInertia;
    float maxSpeed;

    sf::Vector2f acceleration;
    sf::Vector2f velocity;
    sf::Vector2f position;
    sf::Vector2f target;

    float zoomTarget;
    float zoom;
    float zoomVelocity;
    float zoomAcceleration;
    float invZoomInertia;

    sf::RenderWindow& window;
    sf::View& worldView;

    Camera(sf::RenderWindow& _window, sf::View& _worldView) :
        window{_window},
        worldView{_worldView}
    {
        reset();
    }

    void updateCameraPos(float dt)
    {
        acceleration = invInertia * (target - position) - velocity / maxSpeed;

        velocity += acceleration * dt;

        position += velocity * dt;

        zoomAcceleration = invZoomInertia * (zoomTarget - zoom) - zoomVelocity;

        zoomVelocity += zoomAcceleration * dt;

        zoom += zoomVelocity*dt;
    }

    void updateView()
    {
        worldView.setCenter(position);
        worldView.setSize((sf::Vector2f)window.getSize() / zoom);

        window.setView(worldView);
    }

    void reset()
    {
        invInertia = 1.0f / 20.0f;
        maxSpeed = 1.0f;

        acceleration = {0.0f, 0.0f};
        velocity = {0.0f, 0.0f};
        position = {0.0f, 0.0f};
        target = {0000.0f, 0000.0f};

        zoomTarget = 1.0f;
        zoom = 1.0f;
        zoomVelocity = 0.0f;
        zoomAcceleration = 0.0f;
        invZoomInertia = 1.0f/20.f;
    }
};

#endif // CAMERA_H
