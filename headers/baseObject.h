#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

enum class ObjectType
{
    Ball,
    AABB,
    OBB,
    Polygon,
    Capsule,
    _Count,
};

struct Material
{
    std::string shaderID = "";
    std::string diffuseID = "";
    std::string normalID = "";
    std::string emissionID = "";

    float diffuseStrength = 1.0f;
    float ambientStrength = 0.2f;
    float specularStrength = 0.5f;
    float emissionStrength = 0.3f;
    float shininess = 32.0f;
};

struct ObjectProperties
{
    sf::Vector2f _position;
    sf::Vector2f _velocity = {0.0f, 0.0f};
    sf::Vector2f _size = {1.0f, 1.0f};
    float _mass = 1.0f;
    float _coefFric = 0.0f;
    float _coefRest = 0.0f;
    float _rotation = 0.0f;
    float _rotRate = 0.0f;
    bool _isStatic = false;
    bool _bullet = false;
    bool _ignoreGravity = false;
    ObjectType type = ObjectType::Ball;
    std::vector<sf::Vertex > _vertices = {};
    Material material;
};

#endif // BASE_OBJECT_H
