#ifndef BASE_OBJECT_H
#define BASE_OBJECT_H

enum class ObjectType
{
    Ball,
    AABB,
    OBB,
    Polygon,
    Capsule,
    VertexArray,
    ConcavePoly,
    _Count,
};

enum class CollisionIDType
{
    DEFAULT_GEOMETRY = 1,
    CHARACTER_CAPSULE = 2,
    CHARACTER_HITBOXES = 3,
    WEAPON_PROJECTILES = 4
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
    bool castShadow = false;
};

struct ObjectProperties
{
    sf::Vector2f _position;
    sf::Vector2f _velocity = {0.0f, 0.0f};
    sf::Vector2f _size = {1.0f, 1.0f};
    float _mass = 1.0f;
    float _coefFric = 1.0f;
    float _coefRest = 0.0f;
    float _rotation = 0.0f;
    float _rotRate = 0.0f;
    bool _isStatic = false;
    bool _bullet = false;
    bool _ignoreGravity = false;
    bool _enableCollision = true;
    ObjectType type = ObjectType::Ball;
    std::vector<sf::Vertex > _vertices = {};
    Material material;
    float _zPosition = 0.0f;
    int vArrayCount = 0;
    sf::PrimitiveType vArrayType = sf::PrimitiveType::Points;
    std::vector<unsigned int > _collisionGroup = {1};
};

#endif // BASE_OBJECT_H
