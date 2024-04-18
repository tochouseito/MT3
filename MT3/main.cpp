#include <Novice.h>
#include<Matrix4x4.h>
#include <stdio.h>
#include <math.h>
#define N 4 /* 逆行列を求める行列の行数・列数 */

const char kWindowTitle[] = "LE2B_27_ヤラ_チョウセイ";
int check(double mat[N][N], double inv[N][N]) {

	double inner_product;
	int i, j, k;
	double ans;
	double diff;
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			inner_product = 0;
			for (k = 0; k < N; k++) {
				inner_product += mat[i][k] * inv[k][j];
			}

			ans = (i == j) ? 1 : 0;
			diff = ans - inner_product;


		}
	}

	return 1;
}
// 行列の加法
Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = { 0 };
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			result.m[i][j] = m1.m[i][j] + m2.m[i][j];
		}
	}
	return result;

}
// 行列の減法
Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2) {
	Matrix4x4 result = { 0 };
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			result.m[i][j] = m1.m[i][j] - m2.m[i][j];
		}
	}
	return result;
}
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
// 逆行列
Matrix4x4 Inverse(const Matrix4x4& m) {
	Matrix4x4 result = { 0 };
	/* 逆行列を求める行列用の２次元配列 */
	double mat[N][N];

	/* 逆行列用の２次元配列 */
	double inv[N][N];

	/* 掃き出し法を行う行列 */
	double sweep[N][N * 2];

	int i; /* 行 */
	int j; /* 列 */
	int k; /* 注目対角成分が存在する列 */

	double a; /* 定数倍用 */

	/* 正方行列の各成分をセット */
	mat[0][0] = m.m[0][0]; mat[0][1] = m.m[0][1]; mat[0][2] = m.m[0][2]; mat[0][3] = m.m[0][3];
	mat[1][0] = m.m[1][0]; mat[1][1] = m.m[1][1]; mat[1][2] = m.m[1][2]; mat[1][3] = m.m[1][3];
	mat[2][0] = m.m[2][0]; mat[2][1] = m.m[2][1]; mat[2][2] = m.m[2][2]; mat[2][3] = m.m[2][3];
	mat[3][0] = m.m[3][0]; mat[3][1] = m.m[3][1]; mat[3][2] = m.m[3][2]; mat[3][3] = m.m[3][3];

	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			/* sweepの左側に逆行列を求める行列をセット */
			sweep[i][j] = mat[i][j];

			/* sweepの右側に単位行列をセット */
			sweep[i][N + j] = (i == j) ? 1 : 0;
		}
	}


	/* 全ての列の対角成分に対する繰り返し */
	for (k = 0; k < N; k++) {

		/* 最大の絶対値を注目対角成分の絶対値と仮定 */
		double max = fabs(sweep[k][k]);
		int max_i = k;

		/* k列目が最大の絶対値となる行を探す */
		for (i = k + 1; i < N; i++) {
			if (fabs(sweep[i][k]) > max) {
				max = fabs(sweep[i][k]);
				max_i = i;
			}
		}



		/* 操作（１）：k行目とmax_i行目を入れ替える */
		if (k != max_i) {
			for (j = 0; j < N * 2; j++) {
				double tmp = sweep[max_i][j];
				sweep[max_i][j] = sweep[k][j];
				sweep[k][j] = tmp;
			}
		}

		/* sweep[k][k]に掛けると1になる値を求める */
		a = 1 / sweep[k][k];

		/* 操作（２）：k行目をa倍する */
		for (j = 0; j < N * 2; j++) {
			/* これによりsweep[k][k]が1になる */
			sweep[k][j] *= a;
		}

		/* 操作（３）によりk行目以外の行のk列目を0にする */
		for (i = 0; i < N; i++) {
			if (i == k) {
				/* k行目はそのまま */
				continue;
			}

			/* k行目に掛ける値を求める */
			a = -sweep[i][k];

			for (j = 0; j < N * 2; j++) {
				/* i行目にk行目をa倍した行を足す */
				/* これによりsweep[i][k]が0になる */
				sweep[i][j] += sweep[k][j] * a;
			}
		}
	}

	/* sweepの右半分がmatの逆行列 */
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			inv[i][j] = sweep[i][N + j];
		}
	}

	/* 逆行列invを表示 */
	for (i = 0; i < N; i++) {
		for (j = 0; j < N; j++) {
			result.m[i][j] = float(inv[i][j]);
			//printf("%0.2f, ", inv[i][j]);
		}
		//printf("\n");
	}

	/* 検算 */
	if (check(mat, inv)) {
		//printf("invはmatの逆行列です！！\n");
	}
	else {
		//printf("invはmatの逆行列になってません...\n");
	}
	return result;
}
// 転置行列
Matrix4x4 Transpose(const Matrix4x4& m) {
	Matrix4x4 result = { 0 };
	for (int i = 0; i < N; ++i) {
		for (int j = 0; j < N; ++j) {
			result.m[j][i] = m.m[i][j];
		}
	}
	return result;
}
// 単位行列の作成
Matrix4x4 MakeIdentity4x4() {
	Matrix4x4 result = { 0 };
	for (int i = 0; i < N; ++i) {
		result.m[i][i] = 1.0;
	}
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
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	Matrix4x4 m1 = { 3.2f,0.7f,9.6f,4.4f,
				  5.5f,1.3f,7.8f,2.1f,
				  6.9f,8.0f,2.6f,1.0f,
				  0.5f,7.2f,5.1f,3.3f };
	Matrix4x4 m2 = { 4.1f,6.5f,3.3f,2.2f,
				8.8f,0.6f,9.9f,7.7f,
				1.1f,5.5f,6.6f,0.0f,
				3.3f,9.9f,8.8f,2.2f };
	Matrix4x4 resultAdd = Add(m1, m2);
	Matrix4x4 resultMultiply = Multiply(m1, m2);
	Matrix4x4 resultSubtract = Subtract(m1, m2);
	Matrix4x4 inverseM1 = Inverse(m1);
	Matrix4x4 inverseM2 = Inverse(m2);
	Matrix4x4 transposeM1 = Transpose(m1);
	Matrix4x4 transposeM2 = Transpose(m2);
	Matrix4x4 identity = MakeIdentity4x4();
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

		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		MatrixScreenPrintf(0, 0, resultAdd);
		MatrixScreenPrintf(0, kRowHeight * 5, resultSubtract);
		MatrixScreenPrintf(0, kRowHeight * 5 * 2, resultMultiply);
		MatrixScreenPrintf(0, kRowHeight * 5 * 3, inverseM1);
		MatrixScreenPrintf(0, kRowHeight * 5 * 4, inverseM2);
		MatrixScreenPrintf(kColumnWidth * 5, 0, transposeM1);
		MatrixScreenPrintf(kColumnWidth * 5, kRowHeight * 5, transposeM2);
		MatrixScreenPrintf(kColumnWidth * 5, kRowHeight * 5 * 2, identity);
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
