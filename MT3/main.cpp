#include <Novice.h>
#include<Matrix4x4.h>
#include<Vector3.h>
#include <assert.h>
const char kWindowTitle[] = "LE2B_27_ヤラ_チョウセイ";
// １．平行移動行列
Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
	Matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[1][0] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[3][0] = translate.x;
	result.m[0][1] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[2][1] = 0.0f;
	result.m[3][1] = translate.y;
	result.m[0][2] = 0.0f;
	result.m[1][2] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[3][2] = translate.z;
	result.m[0][3] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][3] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}
// ２．拡大縮小行列
Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4 result;
	result.m[0][0] = scale.x;
	result.m[1][0] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[0][1] = 0.0f;
	result.m[1][1] = scale.y;
	result.m[2][1] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[1][2] = 0.0f;
	result.m[2][2] = scale.z;
	result.m[3][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][3] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}
// ３．座標変換
Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix) {
	Vector3 result;
	result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] + vector.z * matrix.m[2][0] + matrix.m[3][0];
	result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] + vector.z * matrix.m[2][1] + matrix.m[3][1];
	result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] + vector.z * matrix.m[2][2] + matrix.m[3][2];
	float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] + vector.z * matrix.m[2][3] + matrix.m[2][3];
	assert(w != 0.0f);
	result.x /= w;
	result.y /= w;
	result.z /= w;
	return result;
}

static const int kRowHeight = 20;
static const int kColumnWidth = 60;
void MatrixScreenPrintf(int x, int y, const Matrix4x4& matrix) {
	for (int row = 0; row < 4; ++row) {
		for (int column = 0; column < 4; ++column) {
			Novice::ScreenPrintf(x + column * kColumnWidth, y + row * kRowHeight, "%6.02f", matrix.m[row][column]);
		}
	}
}
void VectorScreenPrintf(int x, int y, Vector3 vector) {
	Novice::ScreenPrintf(x, y, "%.02f", vector.x);
	Novice::ScreenPrintf(x + kColumnWidth, y, "%.02f", vector.y);
	Novice::ScreenPrintf(x + kColumnWidth * 2, y, "%.02f", vector.z);
}
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

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
		Vector3 translate{ 4.1f,2.6f,0.8f };
		Vector3 scale{ 1.5f,5.2f,7.3f };
		Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
		Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
		Vector3 point{ 2.3f,3.8f,1.4f };
		Matrix4x4 transformMatrix = {
			1.0f,2.0f,3.0f,4.0f,
			3.0f,1.0f,1.0f,2.0f,
			1.0f,4.0f,2.0f,3.0f,
			2.0f,2.0f,1.0f,3.0f
		};
		Vector3 transformed = Transform(point, transformMatrix);



		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		VectorScreenPrintf(0, 0, transformed);
		MatrixScreenPrintf(0, kRowHeight * 5, translateMatrix);
		MatrixScreenPrintf(0, kRowHeight * 5 * 2, scaleMatrix);
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
