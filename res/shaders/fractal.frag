#version 330

in vec3 v_Pos;
out vec4 FragColor;

uniform float u_FractalPower;
uniform float u_AspectRatio;
uniform vec3 u_CameraPosition;
uniform vec3 u_TargetPosition;
uniform int u_Shadows;
uniform int u_FractalId;

//#define u_FractalPower 5.8

//#define u_CameraPosition vec3(-3.0, 0.5, -0.5)
//#define u_TargetPosition vec3(0.0, 0.0, 0.0)

//uniform float u_AspectRatio; // window width / window height
//uniform float tanh_fov; // tanhf(field of view)

//#define u_Shadows 0
//#define u_FractalId 1

//#define u_AspectRatio 1280.0 / 720.0
#define tanh_fov 0.655794202633
#define sun_position vec3(-200.0, 500.0, -300.0)
#define PI 3.1415
#define RAY_STEPS_MAX 128
#define RAY_T_MIN 0.001
#define RAY_T_MAX 5.0

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
    vec3 forward = normalize(u_TargetPosition - u_CameraPosition);
    vec3 right = cross(forward, vec3(0.0, 1.0, 0.0));
    vec3 up = cross(right, forward);

    float h = tanh_fov;
    float w = h * u_AspectRatio;

    Ray r;
    r.origin = u_CameraPosition;
    r.direction = normalize(forward + v_Pos.x * w * right + v_Pos.y * h * up);

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
    return PI*x - x*(abs(x) - 1.0)*(0.2447 + 0.0663*abs(x));
}

float point_fractal_distance(vec3 point)
{
    vec3 z = point;
    float dr = 1.0;
    float r;

    float power = u_FractalPower;

    for (int i = 0; i < 7; i++)
    {
        r = length(z);
        
        if (r > 2.0)
        {
            break;
        }

        float theta = acos(z.z / r) * power;
        float phi = atan(z.y / z.x) * power;
        float zr = pow(r, power);
        dr = pow(r, power - 1.0) * power * dr + 1.0;

        z = zr * vec3(sin(theta) * cos(phi), sin(phi) * sin(theta), cos(theta));
        z += point;
    }

    return 0.5 * log(r) * r / dr;
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

		x = x * 3.0 - 2.0;
        y = y * 3.0 - 2.0;
        z = z * 3.0 - 2.0;

		if (z < -1.0)
        {
            z += 2.0;
        }
	}

	return ((sqrt(x * x + y * y + z * z) - 1.5) * pow(3.0,-iters));
}

float point_box_distance(vec3 point)
{
    return length(max(abs(point - vec3(0.5)), 0.0));
}

float safe_distance(vec3 point)
{
    const Sphere s1 = Sphere(vec3(0.0, 0.5, 0.0), 0.5);

    float sd = RAY_T_MAX;

    if (u_FractalId == 1)
    {
        sd = min (sd, point_fractal_distance(point));
    }
    
    else
    {
        sd = min (sd, point_fractal_distance2(point));
    }

    return sd;
}

float sigmoid(float a)
{
    return 1.0 / (1.0 + exp(-a * 8.0 + 4.0));
}

float calculate_shadow(vec3 point)
{
    Ray r = Ray(point, normalize(sun_position - point));

    float t = 10.0 * RAY_T_MIN;

    float smallest_angle = 1.0;

	for (int i = 0; i < RAY_STEPS_MAX; i++)
	{
		vec3 p = r.origin + r.direction * t;

        float sd = safe_distance(p);

        float angle = atan_fast(sd / t) / (0.5 * PI);
        smallest_angle = min(smallest_angle, angle);

        if (abs(angle) < 0.025)
        {
            return 0.3;
        }

		if (t > RAY_T_MAX)
		{
			return sigmoid(clamp(smallest_angle * 16.0, 0.0, 1.0)) * 0.7 + 0.3;
		}

		else if (sd < RAY_T_MIN)
		{
            return 0.3;
		}

		t += sd;
	}
}

vec3 calculate_color(vec3 point)
{
    float d = max(abs(point.x), max(abs(point.y), abs(point.z)));

    float b = sin((d + 2.24) * 7.9) * 0.5 + 0.5;
    float g = cos((d + 1.78) * 8.0) * 0.5 + 0.5;
    float r = sin((d + 2.78) * 8.1) * 0.5 + 0.5;

    if (u_Shadows == 1)
    {
        return vec3(r, g, b) * calculate_shadow(point);
    }
    
    else
    {
        return vec3(r, g, b);
    }
    
}

void main()
{
    Ray r = ray_create_from_camera();

    vec3 result = vec3(0.0, 0.0, 0.0);

    float t = RAY_T_MIN;

    float min_sd = RAY_T_MAX;

	for (int i = 0; i < RAY_STEPS_MAX; i++)
	{
		vec3 point = r.origin + r.direction * t;

        float sd = safe_distance(point);
        min_sd = min(min_sd, sd);

		if (sd < RAY_T_MIN)
		{
            float ao = 1.0 - i / 200.0;
            result = calculate_color(point) * ao;
			break;
		}

		else if (t > RAY_T_MAX)
		{
            float c = sigmoid(pow(min_sd, 0.25) * 2.0);
            result = mix(vec3(0.9, 0.8, 0.7), vec3(0.1, 0.1, 0.15), c);
			break;
		}

		t += sd;
	}

    FragColor = vec4(result, 1.0);
}