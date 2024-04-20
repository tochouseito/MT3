#include <Novice.h>
#include<Matrix4x4.h>
#include<Vector3.h>
#include <assert.h>
#include<cmath>
#include <math.h>
#define N 4 //逆行列を求める行列の行数・列数 
const char kWindowTitle[] = "LE2B_27_ヤラ_チョウセイ";
// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = { 0 };
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			for (int k = 0; k < N; ++k) {
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}
	return result;
}
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
// ４．X軸回転行列
Matrix4x4 MakeRotateXMatrix(float radian) {
	Matrix4x4 result;
	result.m[0][0] = 1.0f;
	result.m[1][0] = 0.0f;
	result.m[2][0] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[0][1] = 0.0f;
	result.m[1][1] = std::cosf(radian);
	result.m[2][1] = -std::sinf(radian);
	result.m[3][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[1][2] = std::sinf(radian);
	result.m[2][2] = std::cosf(radian);
	result.m[3][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][3] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}
// ５．Y軸回転行列
Matrix4x4 MakeRotateYMatrix(float radian) {
	Matrix4x4 result;
	result.m[0][0] = std::cosf(radian);
	result.m[1][0] = 0.0f;
	result.m[2][0] = std::sinf(radian);
	result.m[3][0] = 0.0f;
	result.m[0][1] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[2][1] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[0][2] = -std::sinf(radian);
	result.m[1][2] = 0.0f;
	result.m[2][2] = std::cosf(radian);
	result.m[3][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][3] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}
// ６．Z軸回転行列
Matrix4x4 MakeRotateZMatrix(float radian) {
	Matrix4x4 result;
	result.m[0][0] = std::cosf(radian);
	result.m[1][0] = -std::sinf(radian);
	result.m[2][0] = 0.0f;
	result.m[3][0] = 0.0f;
	result.m[0][1] = std::sinf(radian);
	result.m[1][1] = std::cosf(radian);
	result.m[2][1] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[1][2] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[3][2] = 0.0f;
	result.m[0][3] = 0.0f;
	result.m[1][3] = 0.0f;
	result.m[2][3] = 0.0f;
	result.m[3][3] = 1.0f;
	return result;
}
// 3次元アフィン変換行列
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate,  const Vector3& translate) {
	Matrix4x4 result;
	Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4 rotateXYZMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);
	result = Multiply(translateMatrix, Multiply(scaleMatrix, rotateXYZMatrix));
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
	Vector3 scale{ 1.2f,0.79f,-2.1f };
	Vector3 rotate{ 0.4f,1.43f,-0.8f };
	Vector3 translate{ 2.7f,-4.15f,1.57f };
	Matrix4x4 worldMatrix = MakeAffineMatrix(scale, rotate, translate);
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
		

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		MatrixScreenPrintf(0, 0, worldMatrix);
		
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
