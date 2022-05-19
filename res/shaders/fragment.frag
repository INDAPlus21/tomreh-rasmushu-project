#version 440

in vec3 position_passed;

out vec4 FragColor;

//uniform float fractal_power;
#define fractal_power 5.8f

//uniform vec3 camera_position;
//uniform vec3 target_position;
#define camera_position vec3(-3.0f, 0.5f, -0.5f)
#define target_position vec3(0.0f, 0.0f, 0.0f)

//uniform float aspect_ratio; // window width / window height
//uniform float tanh_fov; // tanhf(field of view)

#define aspect_ratio 1280.0f / 720.0f
#define tanh_fov 0.655794202633f

//uniform vec3 sun_position;
#define sun_position vec3(-200.0f, 500.0f, -300.0f)

#define PI 3.1415f

#define RAY_STEPS_MAX 128
#define RAY_T_MIN 0.001f
#define RAY_T_MAX 5.0f

struct Ray
{
    vec3 origin;
    vec3 direction;
};

struct Plane
{
    vec3 normal;
    float d;
};

struct Sphere
{
    vec3 origin;
    float radius;
};

struct Box
{
    vec3 origin;
    float side_half;
};

struct Cylinder
{
    vec3 origin;
    float radius;
};

Ray ray_create_from_camera()
{
    vec3 forward = normalize(target_position - camera_position);
    vec3 right = cross(forward, vec3(0.0f, 1.0f, 0.0f));
    vec3 up = cross(right, forward);

    float h = tanh_fov;
    float w = h * aspect_ratio;

    Ray r;
    r.origin = camera_position;
    r.direction = normalize(forward + position_passed.x * w * right + position_passed.y * h * up);

    return r;
}

float point_point_distance(vec3 point1, vec3 point2)
{
    return length(point1 - point2);
}

float point_plane_distance(vec3 point, Plane plane)
{
    return dot(point, plane.normal) + plane.d;
}

float point_sphere_distance(vec3 point, Sphere sphere)
{
    return length(point - sphere.origin) - sphere.radius;
}

float atan_fast(float x)
{
    return PI*x - x*(abs(x) - 1.0f)*(0.2447f + 0.0663f*abs(x));
}

float point_fractal_distance(vec3 point)
{
    vec3 z = point;
    float dr = 1.0f;
    float r;

    float power = fractal_power;

    for (int i = 0; i < 7; i++)
    {
        r = length(z);
        
        if (r > 2.0f)
        {
            break;
        }

        float theta = acos(z.z / r) * power;
        float phi = atan(z.y / z.x) * power;
        float zr = pow(r, power);
        dr = pow(r, power - 1.0f) * power * dr + 1.0f;

        z = zr * vec3(sin(theta) * cos(phi), sin(phi) * sin(theta), cos(theta));
        z += point;
    }

    return 0.5f * log(r) * r / dr;
}

float point_fractal_distance2(vec3 point)
{
    int n,iters = 10;
    float t;

    float x = point.x;
    float y = point.y;
    float z = point.z;

	for (n = 0; n < iters; n++)
    {
		x = abs(x);
        y = abs(y);
        z = abs(z);

		if (x<y)
        {
            t = x;
            x = y;
            y = t;
        }

		if (y < z)
        {
            t = y;
            y = z;
            z = t;
        }

		if (x < y)
        {
            t = x;
            x = y;
            y = t;
        }

		x = x * 3.0f - 2.0f;
        y = y * 3.0f - 2.0f;
        z = z * 3.0f - 2.0f;

		if (z < -1.0)
        {
            z += 2.0f;
        }
	}

	return ((sqrt(x * x + y * y + z * z) - 1.5f) * pow(3.0f,-iters));
}

float point_box_distance(vec3 point)
{
    return length(max(abs(point - vec3(0.5f)), 0.0f));
}

float safe_distance(vec3 point)
{
    //const Plane p1 = Plane(vec3(0.0f, 1.0f, 0.0f), 0.0f);
    const Sphere s1 = Sphere(vec3(0.0f, 0.5f, 0.0f), 0.5f);
    //const Sphere s2 = Sphere(vec3(0.0f, -100000.0f, 0.0f), 100000.0f);
    //const Box b1 = Box(vec3(0.0f, -2.0f, 0.0f), 2.0f);
    //const Cylinder c1 = Cylinder(vec3(-2.0f, 0.0f, 0.0f), 0.5f);

    float sd = RAY_T_MAX;

    //sd = min(sd, point_plane_distance(point, p1));
    //sd = min(sd, point_sphere_distance(point, s1));
    //sd = min(sd, point_sphere_distance(point, s2));
    //sd = min(sd, point_box_distance(point, b1));
    sd = min (sd, point_fractal_distance2(point));

    return sd;
}

float sigmoid(float a)
{
    return 1.0f / (1.0f + exp(-a * 8.0f + 4.0f));
}

float calculate_shadow(vec3 point)
{
    Ray r = Ray(point, normalize(sun_position - point));

    float t = 10.0f * RAY_T_MIN;

    float smallest_angle = 1.0f;

	for (int i = 0; i < RAY_STEPS_MAX; i++)
	{
		vec3 p = r.origin + r.direction * t;

        float sd = safe_distance(p);

        float angle = atan_fast(sd / t) / (0.5f * PI);
        smallest_angle = min(smallest_angle, angle);

        if (abs(angle) < 0.025f)
        {
            return 0.1f;
        }

		if (t > RAY_T_MAX)
		{
			return sigmoid(clamp(smallest_angle * 16.0f, 0.0f, 1.0f)) * 0.9f + 0.1f;
		}

		else if (sd < RAY_T_MIN)
		{
            return 0.1f;
		}

		t += sd;
	}
}

vec3 calculate_color(vec3 point)
{
    float d = max(abs(point.x), max(abs(point.y), abs(point.z)));

    float b = sin((d + 2.24f) * 7.9f) * 0.5f + 0.5f;
    float g = cos((d + 1.78f) * 8.0f) * 0.5f + 0.5f;
    float r = sin((d + 2.78f) * 8.1f) * 0.5f + 0.5f;

    //return vec3(r, g, b);
    return vec3(r, g, b) * calculate_shadow(point);
}

void main()
{
    Ray r = ray_create_from_camera();

    vec3 result = vec3(0.0f, 0.0f, 0.0f);

    float t = RAY_T_MIN;

    float min_sd = RAY_T_MAX;

	for (int i = 0; i < RAY_STEPS_MAX; i++)
	{
		vec3 point = r.origin + r.direction * t;

        float sd = safe_distance(point);
        min_sd = min(min_sd, sd);

		if (sd < RAY_T_MIN)
		{
            float ao = 1.0f - i / 200.0f;
            result = calculate_color(point) * ao;
			break;
		}

		else if (t > RAY_T_MAX)
		{
            float c = sigmoid(pow(min_sd, 0.25f) * 2.0f);
            result = mix(vec3(0.9f, 0.8f, 0.7f), vec3(0.1f, 0.1f, 0.15f), c);
			break;
		}

		t += sd;
	}

    FragColor = vec4(result, 1.0f);
}