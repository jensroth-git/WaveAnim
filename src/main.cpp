#include <r3d.h>
#include <raylib.h>
#include <raymath.h>

static R3D_Mesh plane = {0};
static R3D_Mesh sphere = {0};
static R3D_Material material = {0};
static Camera3D camera = {0};

int main()
{
	// Initialize raylib window
	const auto window_width = 1280;
	const auto window_height = 720;

	SetConfigFlags(FLAG_WINDOW_RESIZABLE);

	InitWindow(window_width, window_height, "Hello Raylib!");
	if (!IsWindowReady()) {
		TraceLog(LOG_ERROR, "Window failed to initialize");
		return 1;
	}

	// Initialize r3d after the window is created (disabled while isolating issue)
	R3D_Init(GetScreenWidth(), GetScreenHeight(), 0);

	plane = R3D_GenMeshPlane(1000, 1000, 1, 1, true);
	sphere = R3D_GenMeshSphere(0.5f, 64, 64, true);
	material = R3D_GetDefaultMaterial();

	camera = (Camera3D) {
		.position = (Vector3) {0, 2, 2},
		.target = (Vector3) {0, 0, 0},
		.up = (Vector3) {0, 1, 0},
		.fovy = 60,
	};

	R3D_Light light = R3D_CreateLight(R3D_LIGHT_SPOT);
	{
		R3D_LightLookAt(light, (Vector3) {0, 10, 5}, (Vector3) {0});
		R3D_EnableShadow(light, 4096);
		R3D_SetLightActive(light, true);
	}

	while (!WindowShouldClose()) {

		UpdateCamera(&camera, CAMERA_ORBITAL);

		BeginDrawing();
		ClearBackground(BLACK);

		R3D_Begin(camera);
            R3D_DrawMesh(&plane, &material, MatrixTranslate(0, -0.5f, 0));
            R3D_DrawMesh(&sphere, &material, MatrixIdentity());
		R3D_End();

		const auto text = "Hello, Raylib and R3D!";
		const auto text_width = MeasureText(text, 20);
		const auto text_height = 20;

		DrawText(text, window_width / 2 - text_width / 2, window_height / 2 - text_height / 2, text_height, RED);
		EndDrawing();
	}

    R3D_UnloadMesh(&plane);
    R3D_UnloadMesh(&sphere);

	R3D_Close();
	CloseWindow();
	return 0;
}
