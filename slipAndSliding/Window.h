//privateメンバ変数のwindowには，開いたウィンドウのハンドル(識別子)を保持する
#pragma once
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//ウィンドウ関連の処理
class Window
{
	//ウィンドウのハンドル
	GLFWwindow* const window;

	//ウィンドウのサイズ
	//描画するウィンドウの幅と高さを保持する二つの要素の配列
	GLfloat size[2];

	//ワールド座標系に対するデバイス座標系の拡大率
	GLfloat scale;

	//縦横比aspectの代わりにscaleを使用する
	////縦横比
	//GLfloat aspect;

	//図形の正規化デバイス座標系上での位置
	GLfloat location[2];

	//キーボードの状態
	int keyStatus;

public:
	//コンストラクタ
	//glfwCreateWindow()関数によりウィンドウを作成した直後は，ビューポートはウィンドウ全体に設定させる
	//scale()(＝見たかったオブジェクトと見たくない背景部分の倍率を変えることができる)
	Window(int width = 640, int height = 480, const char* title = "Which one is more human-like")
		:window(glfwCreateWindow(width, height, title, NULL, NULL))
		, scale(200.0f),location{0.0f, 0.0f},keyStatus(GLFW_RELEASE)
	{
		if (window == NULL)
		{
			//ウィンドウが作成できなかった
			std::cerr << "Cannot create GLFW window" << std::endl;
			exit(1);
		}

		//現在のウィンドウを処理対象にする
		glfwMakeContextCurrent(window);

		//GLEWを初期化する
		glewExperimental = GL_TRUE;
		if (glewInit() != GLEW_OK)
		{
			//GLEWの初期化に失敗した
			std::cerr << "Cannot initialize GLEW" << std::endl;
			exit(1);
		}

		//垂直同期のタイミングを待つ
		//垂直同期とは，ゲーム内のfpsの上限値を，モニタの最高性能までに設定する
		//GPUの性能がモニタのリフレッシュレートを大きく上回った時に発生するティアリング
		//を防ぐ機能．これは，描画途中の画像をディスプレイに表示してしまうことが原因
		glfwSwapInterval(1);

		//このインスタンスwindowに対してpointerに指定したユーザ定義の任意のthisのポインタを記録しておく
		glfwSetWindowUserPointer(window, this);

		//ウィンドウのサイズ変更時に呼び出す処理(コールバック関数)の登録
		//戻り値として，resizeが設定されている
		glfwSetWindowSizeCallback(window, resize);

		//マウスホイール操作時に呼び出す処理の登録
		glfwSetScrollCallback(window, wheel);

		//キーボード操作時に呼び出す処理の登録
		glfwSetKeyCallback(window, keyboard);

		//このインスタンスのthisポインタを記録しておく
		glfwSetWindowUserPointer(window, this);

		//開いたウィンドウの初期設定
		resize(window, width, height);
		
	}
	//デストラクタ=クラスと同じ名前をもつ，メンバ関数で，接頭辞に~(チルダ)をつける．
	virtual ~Window()
	{
		//ウィンドウに設定させているコールバック関数は呼び出されなくなる
		glfwDestroyWindow(window);
	}
	//描画ループの継続判定
	//クラスのコンストラクタの宣言で，explicitをつけると，【1】コピーの初期化ができない，【2】暗黙の型変換ができない
	//explicit: 知らないうちに型が変わっているという心配をする必要がない
	//戻り値の型名は，boolで確定するので，戻り値の型名をoperator演算子の前につける必要はない
	explicit operator bool()
	{
		//イベントを取り出す(イベントが発生するまでプログラムを停止させる)
		//glfwWaitEvents();

		//イベントを取り出す
		//glfwWaitEvents()は，イベントの発生を待つ
		//glfwPollEvents()は，イベントの発生を待たない(＝矢印キーを押した瞬間からスムーズに動き出す)
		//if (keyStatus == GLFW_RELEASE)
		//	glfwWaitEvents();
		//else
		//	glfwPollEvents();

		glfwPollEvents();

		//キーボードの状態を調べる
		//1フレームあたり1画素動かす
		if (glfwGetKey(window, GLFW_KEY_LEFT) != GLFW_RELEASE)
			location[0] -= 2.0f / size[0];
		else if (glfwGetKey(window, GLFW_KEY_RIGHT) != GLFW_RELEASE)
			location[0] += 2.0f / size[0];
		if (glfwGetKey(window, GLFW_KEY_DOWN) != GLFW_RELEASE)
			location[1] -= 2.0f / size[1];
		else if (glfwGetKey(window, GLFW_KEY_UP) != GLFW_RELEASE)
			location[1] += 2.0f / size[1];



		//マウスの左ボタンの状態を調べる
		if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_1) != GLFW_RELEASE)
		{
			//マウスの左ボタンが押されたら，マウスカーソルの位置を取得する
			double x, y;
			glfwGetCursorPos(window, &x, &y);

			//マウスカーソルの正規化デバイス座標系上での位置を求める
			location[0] = static_cast<GLfloat>(x) * 2.0f / size[0] - 1.0f;
			location[1] = 1.0f - static_cast<GLfloat>(y) * 2.0f / size[1];
		}

		//ウィンドウを閉じる必要が無ければTrueを返すで，ウィンドウを閉じる必要がある場合はFalseを返す
		return !glfwWindowShouldClose(window) && !glfwGetKey(window,GLFW_KEY_ESCAPE);
	}

	//ダブルバッファリング
	// void swapBuffers() const{}のようにconstをつけることによって，オブジェクトのメンバ変数を変更することができない．
	//mutable member-variable-declaration;のように，mutableキーワードをつけることによって，
	//クラスの非静的，非定数(static，constでない)のメンバ変数のみ指定できる．
//mutableをつけることによって，メンバ変数に対してデータ変更を行うことができる．
	void swapBuffers() const
	{
		//カラーバッファを入れ替える
		glfwSwapBuffers(window);
	}

	//コールバック関数としてメンバ関数を使う場合には，静的メンバ関数(static)である必要がある
	//ウィンドウのサイズ変更時の処理
	static void resize(GLFWwindow *const window, int width, int height)
	{
		//フレームバッファのサイズを調べ，調べたフレームバッファのサイズの，それぞれ幅と高さをポインタ
		//に格納する
		int fbWidth, fbHeight;
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);

		//フレームバッファ全体をビューポートに設定する
		glViewport(0, 0, fbWidth, fbHeight);

		//このインスタンスのthisポインタを得る
		//glfwGetWindowUserPointer(window)は，記録されたポインタを取り出す対象のウィンドウのハンドル
		Window *const
			instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));

		if (instance != NULL) 
		{
			//開いたウィンドウのサイズを保存する
			instance->size[0] = static_cast<GLfloat>(width);
			instance->size[1] = static_cast<GLfloat>(height);
			//このインスタンスが保持する縦横比を更新する
			//instance->aspect =
			//	static_cast<GLfloat>(width) / static_cast<GLfloat>(height);
		}
	}

	//マウスホイール操作時の処理
	static void wheel(GLFWwindow* window, double x, double y)
	{
		Window* const
			instance(static_cast<Window *>(glfwGetWindowUserPointer(window)));
		if(instance != NULL)
		{
			//ワールド座標系に対するデバイス座標系の拡大率を更新する
			instance->scale += static_cast<GLfloat>(y);
		}
	}

	//キーボード操作時の処理
	//コールバック関数として使うので，静的メンバ関数にする
	static void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		//このインスタンスのthisポインタを得る
		Window *const
			instance(static_cast<Window*>(glfwGetWindowUserPointer(window)));

			if (instance != NULL)
			{
				//キーの状態を保存する
				instance->keyStatus = action;
			}
	}

	//ウィンドウのサイズを取り出す
	//sizeのポインタを取り出すメンバ関数getSize()
	//変数の値をクラス外から参照できるメンバ変数
	const GLfloat* getSize() const { return size; }

	//ワールド座標系に対するデバイス座標系の拡大率を取り出す
	//scaleのポインタを取り出すメンバ関数getScale
	GLfloat getScale() const { return scale; }
	
	//縦横比を取り出す
	//GLfloat getAspect() const { return aspect; }

	//位置を取り出す
	const GLfloat* getLocation() const { return location; }
};