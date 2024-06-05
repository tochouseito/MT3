
#include <Novice.h>
#include"Debug.h"
#include <math.h>
#define _USE_MATH_DEFINES
#include "imgui.h"
const char kWindowTitle[] = "LE2B_27_ヤラ_チョウセイ";
int kWindowWidth = 1280;
int kWindowHeight = 720;

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	
	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };
	Vector3 cameraPosition{ 0.0f, 1.0f, -5.0f };
	//const int kWindowWidth = 1280;
	//const int kWindowHeight = 720;
	/*Sphere sphere;
	sphere.center = { 0, 0, 0 };
	sphere.radius = 1;*/
	Plane plane;
	plane.normal={ 0.0f,1.0f,0.0f };
	plane.distance = 1.0f;
	int planeColor = 0;
	
	Segment segment{ {-2.0f,-1.0f,0.0f},{3.0f,2.0f,2.0f} };
	Vector3 point{ -1.5f,0.6f,0.6f };
	/*Sphere pointSphere{ point,0.01f };*/
	Segment segment2{ {-2.0f,-1.0f,0.0f},{3.0f,2.0f,2.0f} };
	Vector3 project = Project(Subtract(point, segment.origin), segment.diff);
	Vector3 closestPoint = ClosestPoint(point, segment);
	Sphere closestPointSphere{ closestPoint,0.01f };
	Triangle triangle;
	triangle.vertices[0] = { -1.0f,0.0f,0.0f };
	triangle.vertices[1] = { 0.0f,1.0f,0.0f };
	triangle.vertices[2] = { 1.0f,0.0f,0.0f };
	Vector2Int mouse;
	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };
	
	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		
		
		CameraMove(cameraRotate, cameraTranslate, mouse, &keys[DIK_SPACE], &preKeys[DIK_SPACE]);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, Add(cameraPosition, cameraTranslate));
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 ViewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		
		Vector3 start = Transform(Transform(segment.origin, ViewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), ViewProjectionMatrix), viewportMatrix);
		Vector3 start2 = Transform(Transform(segment2.origin, ViewProjectionMatrix), viewportMatrix);
		Vector3 end2 = Transform(Transform(Add(segment2.origin, segment2.diff), ViewProjectionMatrix), viewportMatrix);
		if (IsCollision(triangle,segment2) == true) {
			planeColor = RED;
		} else
		{
			planeColor = WHITE;
		}
		///
		/// ↑更新処理ここまで
		///
		
		///
		/// ↓描画処理ここから
		///
		
		ImGui::Begin("Window");
		ImGui::DragFloat3("CameraTranslate", &cameraTranslate.x, 0.01f);
		ImGui::DragFloat3("CameraRotate", &cameraRotate.x, 0.01f);
		ImGui::DragFloat3("CameraPosition", &cameraPosition.x, 0.01f);
		/*ImGui::DragFloat3("SphereCenter", &sphere.center.x, 0.01f);
		ImGui::DragFloat("SphereRadius", &sphere.radius, 0.01f);*/
		/*ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.01f);
		ImGui::DragFloat("PlaneDistance", &plane.distance, 0.01f);*/
		ImGui::DragFloat3("Triangle[0]", &triangle.vertices[0].x, 0.01f);
		plane.normal = Normalize(plane.normal);
		ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::End();
		DrawGrid(ViewProjectionMatrix, viewportMatrix);
		/*DrawSphere(sphere, ViewProjectionMatrix, viewportMatrix, planeColor);*/
		//DrawPlane(plane, ViewProjectionMatrix, viewportMatrix, planeColor);
		//DrawSphere(pointSphere, ViewProjectionMatrix, viewportMatrix, RED);
		//DrawSphere(closestPointSphere, ViewProjectionMatrix, viewportMatrix, BLACK);
		Novice::DrawLine(int(start2.x), int(start2.y), int(end2.x), int(end2.y), WHITE);
		DrawTriangle(triangle, ViewProjectionMatrix, viewportMatrix, planeColor);
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
