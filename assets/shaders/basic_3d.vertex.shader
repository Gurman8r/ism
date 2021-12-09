#version 460 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_Texcoord;

out _Vertex { vec3 Position; vec3 Normal; vec2 Texcoord; } Vertex;

uniform float Time;
uniform float DeltaTime;
uniform struct _Transform { vec3 Position; Vec3 Scale; Vec4 Rotation; } Transform;
uniform struct _Camera { vec3 Position; vec3 Direction; float Fov; float Near; float Far; vec2 View; } Camera;

mat4 transform(vec3 pos, vec4 rot)
{
    vec3  v = normalize(rot.xyz);
    float s = sin(rot.w);
    float c = cos(rot.w);
    float o = (1. - c);
    return mat4(
        (o * v.x * v.x + c), (o * v.x * v.y - v.z * s), (o * v.z * v.x + v.y * s), 0.,
        (o * v.x * v.y + v.z * s), (o * v.y * v.y + c), (o * v.y * v.z - v.x * s), 0.,
        (o * v.z * v.x - v.y * s), (o * v.y * v.z + v.x * s), (o * v.z * v.z + c), 0.,
        pos.x, pos.y, pos.z, 1.
    );
}

mat4 look_at(vec3 pos, vec3 dir)
{
    vec3 up = vec3(0, 1, 0);
    vec3 center = pos + normalize(dir);
    vec3 f = normalize(center - pos);
    vec3 s = normalize(cross(f, up));
    vec3 u = cross(s, f);
    mat4 m = mat4(1.);
    m[0][0] = s.x; m[1][0] = s.y; m[2][0] = s.z;
    m[0][1] = u.x; m[1][1] = u.y;  m[2][1] = u.z;
    m[0][2] = -f.x; m[1][2] = -f.y; m[2][2] = -f.z;
    m[3][0] = -dot(s, pos); m[3][1] = -dot(u, pos); m[3][2] = dot(f, pos);
    return m;
}

mat4 perspective(float fov, float aspect, float near, float far)
{
    return mat4(
        1. / (aspect * tan(fov / 2.)), 0., 0., 0.,
        0., 1. / tan(fov / 2.), 0., 0.,
        0., 0., -(far + near) / (far - near), -1.,
        0., 0., -(2. * far * near) / (far - near), 0.
    );
}

void main()
{
    mat4 m = transform(Transform.Position, vec4(Transform.Rotation.xyz, Transform.Rotation.w * Time));
    mat4 v = look_at(Camera.Position, Camera.Direction);
    mat4 p = perspective(Camera.Fov, (Camera.View.x / Camera.View.y), Camera.Near, Camera.Far);
    gl_Position = (p * v * m) * vec4(a_Position * Transform.Scale, 1.0);
    Vertex.Position = gl_Position.xyz;
    Vertex.Normal = (transpose(inverse(m)) * vec4(a_Normal, 1.0)).xyz;
    Vertex.Texcoord = a_Texcoord;
}