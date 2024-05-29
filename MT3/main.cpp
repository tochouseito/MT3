
#include <Novice.h>
#include"Matrix.h"
#include <math.h>
#define _USE_MATH_DEFINES
#include "imgui.h"
const char kWindowTitle[] = "LE2B_27_ヤラ_チョウセイ";
int kWindowWidth = 1280;
int kWindowHeight = 720;
struct Sphere {
	Vector3 center; // !< 中心点
	float radius;   // !< 半径
};
struct Line {
	Vector3 origin; // !<始点
	Vector3 diff;   // !<終点への差分ベクトル
};
struct Ray {
	Vector3 origin; // !<始点
	Vector3 diff;   // !<終点への差分ベクトル
};
struct Segment {
	Vector3 origin; // !<始点
	Vector3 diff;   // !<終点への差分ベクトル
};
struct Plane {
	Vector3 normal; //!< 法線
	float distance; //!< 距離
};

void DrawGrid(const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix) {
	const float kGridHalfWidth = 2.0f;                                      // グリッドの半分の幅
	const uint32_t kSubdivision = 10;                                       // 分割数
	const float kGridEvery = (kGridHalfWidth * 2.0f) / float(kSubdivision); // 1つの長さ

	// 奥から手前への線を順々に引いていく
	for (uint32_t xIndex = 0; xIndex <= kSubdivision; ++xIndex) {
		// ワールド座標系上の始点と終点を求める
		Vector3 worldStartPos = { -kGridHalfWidth + xIndex * kGridEvery, 0.0f, -kGridHalfWidth };
		Vector3 worldEndPos = { -kGridHalfWidth + xIndex * kGridEvery, 0.0f, kGridHalfWidth };

		// ビュープロジェクションマトリックスを使ってクリップ座標系に変換
		Vector3 clipStartPos = Transform(worldStartPos, viewProjectionMatrix);
		Vector3 clipEndPos = Transform(worldEndPos, viewProjectionMatrix);

		// クリップ座標系からスクリーン座標系に変換
		Vector3 screenStartPos = Transform(clipStartPos, viewportMatrix);
		Vector3 screenEndPos = Transform(clipEndPos, viewportMatrix);

		// 真ん中の線を黒で描画
		if (xIndex == kSubdivision / 2 && kSubdivision % 2 == 0) {
			Novice::DrawLine(int(screenStartPos.x), int(screenStartPos.y), int(screenEndPos.x), int(screenEndPos.y), 0x000000FF); // 黒色で描画
		}
		else {
			Novice::DrawLine(int(screenStartPos.x), int(screenStartPos.y), int(screenEndPos.x), int(screenEndPos.y), 0xAAAAAAFF); // グレーで描画
		}
	}

	// 左右の線を引くためにyIndexのループも同様に処理
	for (uint32_t yIndex = 0; yIndex <= kSubdivision; ++yIndex) {
		// ワールド座標系上の始点と終点を求める
		Vector3 worldStartPos = { -kGridHalfWidth, 0.0f, -kGridHalfWidth + yIndex * kGridEvery };
		Vector3 worldEndPos = { kGridHalfWidth, 0.0f, -kGridHalfWidth + yIndex * kGridEvery };

		// ビュープロジェクションマトリックスを使ってクリップ座標系に変換
		Vector3 clipStartPos = Transform(worldStartPos, viewProjectionMatrix);
		Vector3 clipEndPos = Transform(worldEndPos, viewProjectionMatrix);

		// クリップ座標系からスクリーン座標系に変換
		Vector3 screenStartPos = Transform(clipStartPos, viewportMatrix);
		Vector3 screenEndPos = Transform(clipEndPos, viewportMatrix);

		// 真ん中の線を黒で描画
		if (yIndex == kSubdivision / 2 && kSubdivision % 2 == 0) {
			Novice::DrawLine(int(screenStartPos.x), int(screenStartPos.y), int(screenEndPos.x), int(screenEndPos.y), 0x000000FF); // 黒色で描画
		}
		else {
			Novice::DrawLine(int(screenStartPos.x), int(screenStartPos.y), int(screenEndPos.x), int(screenEndPos.y), 0xAAAAAAFF); // グレーで描画
		}
	}
}

//3次元ベクトル a をベクトル b に正射影する関数
Vector3 Project(const Vector3& v1, const Vector3& v2) {
	float dot = Dot(v1, v2);
	float magSquared = MagnitudeSquared(v2);
	float scalar = dot / magSquared;
	return { v2.x * scalar, v2.y * scalar, v2.z * scalar };
}
//最近接点を計算する関数
Vector3 ClosestPoint(const Vector3& point, const Segment& segment) {
	Vector3 pointOnLine = segment.origin; // 直線上の任意の点はセグメントの始点と同じと仮定
	float dot = Dot(segment.diff, Subtract(point, pointOnLine));
	float magSquared = MagnitudeSquared(segment.diff);
	float t = dot / magSquared;
	return { pointOnLine.x + segment.diff.x * t, pointOnLine.y + segment.diff.y * t, pointOnLine.z + segment.diff.z * t };
}
void DrawSphere(const Sphere& sphere, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMarix, uint32_t color) {
	const uint32_t kSubdivision = 10;                          // 分割数
	const float kLonEvery = 2.0f * float(M_PI) / kSubdivision; // 経度分割1つ分の角度
	const float kLatEvery = float(M_PI) / kSubdivision;        // 緯度分割1つ分の角度

	// 緯度の方向に分割　-π/2 ～ π/2
	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex) {
		float lat = -float(M_PI) / 2.0f + kLatEvery * latIndex; // 現在の緯度

		// 経度の方向に分割 0 ～ 2π
		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex) {
			float lon = lonIndex * kLonEvery; // 現在の経度

			// 現在の点を求める
			float x1 = sphere.center.x + sphere.radius * std::cosf(lat) * std::cosf(lon);
			float y1 = sphere.center.y + sphere.radius * std::sinf(lat);
			float z1 = sphere.center.z + sphere.radius * std::cosf(lat) * std::sinf(lon);

			// 次の点を求める（経度方向）
			float x2 = sphere.center.x + sphere.radius * std::cosf(lat) * std::cosf(lon + kLonEvery);
			float y2 = sphere.center.y + sphere.radius * std::sinf(lat);
			float z2 = sphere.center.z + sphere.radius * std::cosf(lat) * std::sinf(lon + kLonEvery);

			// 次の点を求める（緯度方向）
			float x3 = sphere.center.x + sphere.radius * std::cosf(lat + kLatEvery) * std::cosf(lon);
			float y3 = sphere.center.y + sphere.radius * std::sinf(lat + kLatEvery);
			float z3 = sphere.center.z + sphere.radius * std::cosf(lat + kLatEvery) * std::sinf(lon);

			// 3D座標をVector3にセット
			Vector3 start(x1, y1, z1);
			Vector3 end1(x2, y2, z2);
			Vector3 end2(x3, y3, z3);

			// 座標変換を行う
			start = Transform(start, viewProjectionMatrix);
			start = Transform(start, viewportMarix);
			end1 = Transform(end1, viewProjectionMatrix);
			end1 = Transform(end1, viewportMarix);
			end2 = Transform(end2, viewProjectionMatrix);
			end2 = Transform(end2, viewportMarix);

			// 線を描画
			Novice::DrawLine(int(start.x), int(start.y), int(end1.x), int(end1.y), color);
			Novice::DrawLine(int(start.x), int(start.y), int(end2.x), int(end2.y), color);
		}
	}
}
bool IsCollision(const Sphere& s1, const Sphere& s2) {
	// 2つの円の中心間の距離を計算
	float distance = float(std::sqrt(std::pow(s2.center.x - s1.center.x, 2) + std::pow(s2.center.y - s1.center.y, 2) + std::pow(s2.center.z - s1.center.z, 2)));
	// 中心間の距離が2つの円の半径の合計よりも小さい場合、衝突しているとみなす
	if (distance <= (s1.radius + s2.radius)) {
		return true;
	} else {
		return false;
	}
}
bool IsCollision(const Sphere& sphere, const Plane& plane) {
	// 2つの円と平面の中心間の距離を計算
	float distance = std::abs((Dot(plane.normal, sphere.center))-plane.distance);
	//float distance = float(std::sqrt(std::pow(plane.normal.x - sphere.center.x, 2) + std::pow(plane.normal.y - sphere.center.y, 2) + std::pow(plane.normal.z - sphere.center.z, 2)));
	// 中心間の距離が2つの円の半径の合計よりも小さい場合、衝突しているとみなす
	if (distance <= sphere.radius) {
		return true;
	} else {
		return false;
	}
}
bool IsCollision(const Segment& segment, const Plane& plane) {
	// まず垂直判定を行うために、法線と線の内積を求める
	float dot = Dot(plane.normal, segment.diff);
	// 垂直＝平行であるので、衝突しているはずがない
	if (dot == 0.0f) {
		return false;
	}
	// ｔを求める
	float t = (plane.distance - Dot(segment.origin, plane.normal)) / dot;

	// 
	//float distance = Dot(segment.origin + (segment.diff * t), plane.normal);
	if (t>=0.0f&&t<=1.0f) {
		return true;
	} else {
		return false;
	}
}
Vector3 Perpendicular(const Vector3& vector) {
	if (vector.x != 0.0f || vector.y != 0.0f) {
		return{ -vector.y,vector.x,0.0f };
	}
	return{ 0.0f,-vector.z,vector.y };
}
void DrawPlane(const Plane& plane, const Matrix4x4& viewProjectionMatrix, const Matrix4x4& viewportMatrix, uint32_t color) {
	Vector3 center = Multiply(plane.distance, plane.normal);//1
	Vector3 perpendiculars[4];
	perpendiculars[0] = Normalize(Perpendicular(plane.normal));//2
	perpendiculars[1] = { -perpendiculars[0].x,-perpendiculars[0].y,-perpendiculars[0].z };//3
	perpendiculars[2] = Cross(plane.normal, perpendiculars[0]);//4
	perpendiculars[3] = { -perpendiculars[2].x ,- perpendiculars[2].y,-perpendiculars[2].z };//5
	//6
	Vector3 points[4];
	for (int32_t index = 0; index < 4; ++index) {
		Vector3 extend = Multiply(2.0f, perpendiculars[index]);
		Vector3 point = Add(center, extend);
		points[index] = Transform(Transform(point, viewProjectionMatrix), viewportMatrix);
	}
	// pointsをそれぞれ結んでDrawLineで矩形を描画する。DrawTriangleを使って塗りつぶしてもいいが、DepthがないのでMT3ではわかりづらい
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[2].x), int(points[2].y), color);
	Novice::DrawLine(int(points[0].x), int(points[0].y), int(points[3].x), int(points[3].y), color);
	Novice::DrawLine(int(points[1].x), int(points[1].y), int(points[3].x), int(points[3].y), color);
}
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
		

		/*
		int mouseX = 0, mouseY = 0;
		if (Novice::IsTriggerMouse(0)) {
			Novice::GetMousePosition(&mouseX, &mouseY);
		}
		if (Novice::IsPressMouse(0)) {
			static int lastMouseX, lastMouseY;
			Novice::GetMousePosition(&lastMouseX, &lastMouseY);
			int dx = mouseX - lastMouseX;
			int dy = mouseY - lastMouseY;
			lastMouseX = mouseX;
			lastMouseY = mouseY;
			cameraRotate.y += float(dx*0.001f);
			cameraRotate.x += float(dy * 0.001f);
			mouseX = lastMouseX;
			mouseY = lastMouseY;
			Novice::GetMousePosition(&mouseX, &mouseY);
		}
		*/

		cameraPosition.z+= Novice::GetWheel()/100;
		Matrix4x4 cameraMatrix = MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, Add(cameraPosition, cameraTranslate));
		Matrix4x4 viewMatrix = Inverse(cameraMatrix);
		Matrix4x4 projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		Matrix4x4 ViewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
		Matrix4x4 viewportMatrix = MakeViewportMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);
		Vector3 start = Transform(Transform(segment.origin, ViewProjectionMatrix), viewportMatrix);
		Vector3 end = Transform(Transform(Add(segment.origin, segment.diff), ViewProjectionMatrix), viewportMatrix);
		Vector3 start2 = Transform(Transform(segment2.origin, ViewProjectionMatrix), viewportMatrix);
		Vector3 end2 = Transform(Transform(Add(segment2.origin, segment2.diff), ViewProjectionMatrix), viewportMatrix);
		if (IsCollision(segment2,plane) == true) {
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
		ImGui::DragFloat3("Plane.Normal", &plane.normal.x, 0.01f);
		ImGui::DragFloat("PlaneDistance", &plane.distance, 0.01f);
		plane.normal = Normalize(plane.normal);
		ImGui::InputFloat3("Project", &project.x, "%.3f", ImGuiInputTextFlags_ReadOnly);
		ImGui::End();
		DrawGrid(ViewProjectionMatrix, viewportMatrix);
		/*DrawSphere(sphere, ViewProjectionMatrix, viewportMatrix, planeColor);*/
		DrawPlane(plane, ViewProjectionMatrix, viewportMatrix, planeColor);
		//DrawSphere(pointSphere, ViewProjectionMatrix, viewportMatrix, RED);
		//DrawSphere(closestPointSphere, ViewProjectionMatrix, viewportMatrix, BLACK);
		Novice::DrawLine(int(start2.x), int(start2.y), int(end2.x), int(end2.y), WHITE);
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
