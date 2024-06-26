#define NOMINMAX
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
	AABB aabb[1];
	aabb[0].min = { -0.5f,-0.5f,-0.5f };
	aabb[0].max = { 0.5f,0.5f,0.5f };
	/*OBB obb{
		.center{-1.0f,0.0f,0.0f},
		.orientations = {{1.0f,0.0f,0.0f},
		{0.0f,1.0f,0.0f},
		{0.0f,0.0f,1.0f}},
		.size{0.5f,0.5f,0.5f}
	};*/
	OBB obb1;
	obb1.center = Vector3(0, 0, 0);
	obb1.size = Vector3(1, 1, 1);
	obb1.orientations[0] = Vector3(1, 0, 0);
	obb1.orientations[1] = Vector3(0, 1, 0);
	obb1.orientations[2] = Vector3(0, 0, 1);

	OBB obb2;
	obb2.center = Vector3(2, 0, 0);
	obb2.size = Vector3(1, 1, 1);
	obb2.orientations[0] = Vector3(1, 0, 0);
	obb2.orientations[1] = Vector3(0, 1, 0);
	obb2.orientations[2] = Vector3(0, 0, 1);
	int AABBColor = 0;
	static bool isDebugCamera = false;
	Vector3 rotate{ 0.0f,0.0f,0.0f };
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
		
		if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
			isDebugCamera = !isDebugCamera;
		}
		CameraMove(cameraRotate, cameraTranslate, mouse, isDebugCamera);
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, Add(cameraPosition, cameraTranslate));
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 ViewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		// 回転行列を生成
		Matrix4x4 rotateMatrix = Multiply(MakeRotateXMatrix(rotate.x), Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));

		// 回転行列から軸を抽出
		for (int i = 0; i < 3; ++i) {
			obb1.orientations[i].x = rotateMatrix.m[i][0];
			obb1.orientations[i].y = rotateMatrix.m[i][1];
			obb1.orientations[i].z = rotateMatrix.m[i][2];
		}
		 // 45度回転
		//RotateOBB(obb2, rotate.x, rotate.y, rotate.z);

		
		if (OBBIntersect(obb1, obb2)) {
			AABBColor = RED;
		} else
		{
			AABBColor = WHITE;
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
		ImGui::DragFloat3("OBB:center", &obb1.center.x, 0.01f);
		
		ImGui::DragFloat3("OBBRotate", &rotate.x, 0.01f);
		ImGui::End();
		
		//DrawSphere(sphere, ViewProjectionMatrix, viewportMatrix, WHITE);
		DrawOBB(obb1, ViewProjectionMatrix, viewportMatrix, AABBColor);
		DrawOBB(obb2, ViewProjectionMatrix, viewportMatrix, WHITE);
		//Novice::DrawLine(int(start.x), int(start.y), int(end.x), int(end.y), WHITE);

		DrawGrid(ViewProjectionMatrix, viewportMatrix);
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
