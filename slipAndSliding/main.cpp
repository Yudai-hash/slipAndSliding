//#pragma comment(lib,"openg132.lib")
//#pragma comment(lib,"glfw3.lib")
//#pragma comment(lib,"libglew32d.lib")
//#pragma comment(linker,"/subsystem:s\"windows\" /entry:\"mainCRTStartup\"")
//
//#define GLEW_STATIC
//#include <GL/glew.h>

#include <cstdlib> //atexit()関数を定義しているヘッダファイルcstdlibを#includeする
#include <iostream>
#include <fstream> //ファイルの入出力を行う
#include <vector> //C++の標準テンプレートライブラリに含まれるvectorを使用する
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Shape.h"

//シェーダオブジェクトのコンパイル結果を表示する
//shader: シェーダオブジェクト名
//str: コンパイルエラーが発生した場所を示す文字列
GLboolean printShaderInfoLog(GLuint shader, const char* str)
{
	//コンパイル結果を取得する
	GLint status;
	//シェーダオブジェクトの情報を取り出す，そしてstatusに格納する
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE) std::cerr << "Compile Error in" << std::endl;

	//シェーダのコンパイル時のログの長さを取得する
	GLsizei bufSize;
	glGetShaderiv(shader,GL_INFO_LOG_LENGTH,&bufSize);

	if (bufSize > 1) {
		//シェーダのコンパイル時のログの内容を取得する
		std::vector<GLchar> infoLog(bufSize);
		GLsizei length;
		//取り出すログの最大の長さが，bufSizeより小さくなければいけない
		//取り出したログの実際の長さの格納先
		//infoLog[0]に取り出したログを格納する
		glGetShaderInfoLog(shader,bufSize, &length, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}

	return static_cast<GLboolean>(status);
}

//プログラムオブジェクトのリンク結果を表示する
//program: プログラムオブジェクト名
GLboolean printProgramInfoLog(GLuint program)
{
	//リンク結果を取得する
	GLint status;
	//プログラムオブジェクトの情報を取り出す
	//programに指定したプログラムオブジェクトのリンクが成功したかどうかを調べて，statusに格納する
	glGetProgramiv(program, GL_LINK_STATUS, & status);
	if (status == GL_FALSE) std::cerr << "Link Error." << std::endl;

	//シェーダのリンク時のログの長さを取得する
	GLsizei bufSize;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufSize);

	if(bufSize > 1)
	{
		//シェーダのリンク時のログの内容を取得する
		std::vector<GLchar> infoLog(bufSize);
		GLsizei lenghth;
		//シェーダオブジェクトのコンパイル時のログを取り出す
		glGetProgramInfoLog(program, bufSize, &lenghth, &infoLog[0]);
		std::cerr << &infoLog[0] << std::endl;
	}
	return static_cast<GLboolean>(status);
}

//プログラムオブジェクトを作成する
//vsrc:バーテックスシェーダのソースプログラムの文字列
//fsrc:フラグメントシェーダのソースプログラムの文字列
GLuint createProgram(const char* vsrc, const char* fsrc) {
	//空のプログラムオブジェクトを作成する
	//glCreateProgram()の戻り値は，作成されたプログラムオブジェクト名(番号)
	const GLuint program(glCreateProgram());

	if (vsrc != NULL) {
		//バーテックシェーダのシェーダオブジェクトを作成する
		//戻り値は，作成されたシェーダオブジェクト名(番号)
		const GLuint vobj(glCreateShader(GL_VERTEX_SHADER));

		glShaderSource(vobj, 1, &vsrc, NULL);
		glCompileShader(vobj);

		//バーテックスシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
		//シェーダのコンパイル直後のシェーダオブジェクトに対してエラーをチェックする
		if (printShaderInfoLog(vobj, "vertex shader"))
			glAttachShader(program, vobj);
		glDeleteShader(vobj);
	}

	//ソースプログラムの文字列がNULLのときは，シェーダオブジェクトを作成しないようにする
	if (fsrc != NULL) {
		//フラグメントシェーダのシェーダオブジェクトを作成する
		const GLuint fobj(glCreateShader(GL_FRAGMENT_SHADER));
		//シェーダのソースプログラムを読み込む
		//1は，引数stringに指定した配列の要素数
		glShaderSource(fobj, 1, &fsrc, NULL);
		//シェーダオブジェクトに読み込まれたソースファイルをコンパイルする
		glCompileShader(fobj);

		//フラグメントシェーダのシェーダオブジェクトをプログラムオブジェクトに組み込む
		if(printShaderInfoLog(fobj,"fragment shader"))
			glAttachShader(program, fobj);
		//使うあてがないので，シェーダオブジェクトを削除(削除マークをつける)
		glDeleteShader(fobj);
	}

	//プログラムオブジェクトをリンクする
	glBindAttribLocation(program, 0, "position");
	//フラグメントシェーダのソースプログラム中のout変数にカラーバッファを割り当てる
	glBindFragDataLocation(program, 0, "fragment");
	glLinkProgram(program);

	//作成したプログラムオブジェクトを返す
	if(printProgramInfoLog(program))
		return program;

	//プログラムオブジェクトが作成できなければ0を返す
	glDeleteProgram(program);
	return 0;
}

//シェーダのソースファイルを読み込んだメモリを返す
//name: シェーダのソースファイル名
//buffer: 読み込んだソースファイルのテキスト
bool readShaderSource(const char* name, std::vector<GLchar>& buffer)
{
	//ファイル名がNULLだった
	if (name == NULL) return false;

	//ソースファイルを開く
	std::ifstream file(name, std::ios::binary);
	if (file.fail())
	{
		//開けなかった
		std::cerr << "Error: Cannot open source file: " << name << std::endl;
		return false;
	}

	//ファイルの末尾に移動し現在位置(＝ファイルサイズ)を得る
	file.seekg(0L, std::ios::end);
	GLsizei length = static_cast<GLsizei>(file.tellg());

	//ファイルサイズのメモリを確保
	buffer.resize(length + 1);

	//ファイルを先頭から読み込む
	file.seekg(0L, std::ios::beg);
	file.read(buffer.data(), length);
	buffer[length] = '\0';

	if (file.fail())
	{
		//うまく読み込めなかった
		std::cerr << "Error: Could not read source file" << name << std::endl;
		file.close();
		return false;
	}

	//読み込み成功
	file.close();
	return true;
}

//readShaderSource()でソースファイルを読み込み，createProgram()を使用して，プログラムオブジェクトを作成する関数
//vert: バーテックスシェーダのソースファイル名
//frag: フラグメントシェーダのソースファイル名
GLuint loadProgram(const char* vert, const char* frag)
{
	//シェーダのソースフィイルを読み込む
	std::vector<GLchar> vsrc;
	const bool vstat(readShaderSource(vert, vsrc));
	std::vector<GLchar> fsrc;
	const bool fstat(readShaderSource(frag, fsrc));

	//プログラムオブジェクトを作成する
	//式1 ? 式2 : 式3は，式1が評価され，trueなら式2が評価され，falseなら式3が評価される．
	return vstat && fstat ? createProgram(vsrc.data(), fsrc.data()) : 0;
}

//矩形の頂点の位置
constexpr Object::Vertex rectangleVertex[] =
{
	{-0.5f,-0.5f},
	{1.5f,-0.5f},
	{1.5f,1.5f},
	{-0.5f,1.5f}
};

int main() {
	//GLFWを初期化する
	//
	if (glfwInit() == GL_FALSE)
	{
		//初期化に失敗した
		std::cerr << "Cannot initialize GLFW" << std::endl;
		return 1;
	}

	//プログラム修了時の処理を登録する
	atexit(glfwTerminate);

	//OpenGL Version 3.2 Core Profileを選択する(グラフィックハードウェアがglfwWindowHint()関数で指定した特性に対応していなければ，glfwCreateWindow()関数の実行は失敗してウィンドウは開かれない)
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT,GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	//ウィンドウを作成する
	GLFWwindow* const window(glfwCreateWindow(640, 480, "Hello", NULL, NULL));
	if (window == NULL) {
		//ウィンドウが作成できなかった
		std::cerr << "Cannot create GLFW window." << std::endl;
		return 1;
	}

	//作成したウィンドウをOpenGLの処理対象にする
	glfwMakeContextCurrent(window);

	//GLEWを初期化する
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		//GLEWの初期化に失敗した
		std::cerr << "Cannot initialize GLEW" << std::endl;
		return 1;
	}

	//垂直同期のタイミングを待つ
	glfwSwapInterval(1);

	//背景色を指定する
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	//デバイス座標形状にビューポートを設定する
	glViewport(100, 50, 300, 300);

	//シェーダのソースファイルは，C++のソースファイルとは別に作成した
	//バーテックスシェーダのソースプログラム
	//static constexpr GLchar vsrc[] =
	//	"#version 150 core\n"
	//	"in vec4 position;\n"
	//	"void main()\n"
	//	"{\n"
	//	" gl_Position = position;\n"
	//	"}\n";

	//フラグメントシェーダのソースプログラム
	//画素に赤色を設定
	//フラグメントの色の出力先のout変数をfragmentという変数名で宣言
	//static constexpr GLchar fsrc[] =
	//   	"#version 150 core\n"
	//	"out vec4 fragment;\n"
	//	"void main()\n"
	//	"{\n"
	//	" fragment = vec4(1.0, 0.0, 0.0, 1.0);\n"
	//	"}\n";

	//プログラムオブジェクトを作成する
	const GLuint program(loadProgram("point.vert", "point.frag"));

	//図形データを作成する
	std::unique_ptr<const Shape> shape(new Shape(2, 4, rectangleVertex));
	
	//ウィンドウが開いている間繰り返す
	while (glfwWindowShouldClose(window) == GL_FALSE)
	{
		//ウィンドウを消去する
		glClear(GL_COLOR_BUFFER_BIT);

		//シェーダオブジェクトの使用開始(図形の描写に使用するプログラムオブジェクトを指定する)
		glUseProgram(program);


		//図形を描画する
		shape->draw();

		//カラーバッファを入れ替える
		glfwSwapBuffers(window);

		//イベントを取り出す
		glfwWaitEvents();
	}
}

