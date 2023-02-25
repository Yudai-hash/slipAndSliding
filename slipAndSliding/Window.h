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

public:
	//コンストラクタ
	Window(int width = 640, int height = 480, const char* title = "Hello")
		:window(glfwCreateWindow(width, height, title, NULL, NULL))
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
		glfwSwapInterval(1);

		//ウィンドウのサイズ変更時に呼び出す処理(コールバック関数)の登録
		//戻り値として，resizeが設定されている
		glfwSetWindowSizeCallback(window, resize);

		//開いたウィンドウの初期設定
		resize(window, width, height);
		
	}
	//デストラクタ
	virtual ~Window()
	{
		//ウィンドウに設定させているコールバック関数は呼び出されなくなる
		glfwDestroyWindow(window);
	}
	//描画ループの継続判定
	explicit operator bool()
	{
		//イベントを取り出す
		glfwWaitEvents();

		//ウィンドウを閉じる必要が無ければtrueを返す
		return !glfwWindowShouldClose(window);
	}

	//ダブルバッファリング
	void swapBuffers() const
	{
		//カラーバッファを入れ替える
		glfwSwapBuffers(window);
	}
};