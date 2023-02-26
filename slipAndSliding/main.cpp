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
#include "Window.h"
#include "Matrix.h"
#include "Shape.h"
#include "ShapeIndex.h"
#include "SolidShapeIndex.h"
#include "SolidShape.h"

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
	//バーテックスシェーダのattribute変数colorを追加する．colorのattribute変数の場所は，1とする．
	glBindAttribLocation(program, 1, "color");
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

//八面体の頂点の位置
//頂点の数＝面の数×一つの面に含まれる辺の数÷一つの頂点を共有する面の数＝8×3÷4=6
//一筆書きしたときに12回頂点を指定するから？
constexpr Object::Vertex octahedronVertex[] =
{
	{0.0f, 1.0f, 0.0f},
	{-1.0f, 0.0f, 0.0f},
	{0.0f, -1.0f, 0.0f},
	{1.0f, 0.0f, 0.0f},
	{0.0f, 1.0f, 0.0f},
	{0.0f, 0.0f, 1.0f},
	{0.0f, -1.0f, 0.0f},
	{0.0f, 0.0f, -1.0f},
	{-1.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 1.0f},
	{1.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, -1.0f}
};

//六面体の頂点の位置
constexpr Object::Vertex cubeVertex[] =
{
	{-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f}, //(0)
	{-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.8f}, //(1)
	{-1.0f,  1.0f, 1.0f, 0.0f, 0.8f, 0.0f}, //(2)
	{-1.0f,  1.0f, -1.0f, 0.0f, 0.8f, 0.8f}, //(3)
	{ 1.0f,  1.0f, -1.0f, 0.8f, 0.0f, 0.0f}, //(4)
	{ 1.0f, -1.0f, -1.0f, 0.8f, 0.0f, 0.8f}, //(5)
	{ 1.0f, -1.0f,  1.0f, 0.8f, 0.8f, 0.0f}, //(6)
	{ 1.0f,  1.0f,  1.0f, 0.8f, 0.8f, 0.8f} //(7)
};

//六面体の稜線の両端点のインデックス
constexpr GLuint wireCubeIndex[] =
{
	1, 0, //(a)
	2, 7, //(b)
	3, 0, //(c)
	4, 7, //(d)
	5, 0, //(e)
	6, 7, //(f)
	1, 2, //(g)
	2, 3, //(h)
	3, 4, //(i)
	4, 5, //(j)
	5, 6, //(k)
	6, 1, //(l)
};

//六面体の面を塗りつぶす三角形の頂点のインデックス
//constexpr GLuint solidCubeIndex[] =
//{
//	0, 1, 2, 0, 2, 3, //左
//	0, 3, 4, 0, 4, 5, //裏
//	0, 5, 6, 0, 6, 1, //下
//	7, 6, 5, 7, 5, 4, //右
//	7, 4, 3, 7, 3, 2, //上
//	7, 2, 1, 7, 1, 6 //前
//};

//面ごとに色を変えた六面体の頂点属性
constexpr Object::Vertex solidCubeVertex[] =
{
	//左
	{-1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.1f},
	{-1.0f, -1.0f, 1.0f, 0.1f, 0.8f, 0.1f},
	{-1.0f, 1.0f, 1.0f, 0.1f, 0.8f, 0.1f},
	{-1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.1f},
	{-1.0f, 1.0f, 1.0f, 0.1f, 0.8f, 0.1f},
	{-1.0f, 1.0f, -1.0f, 0.1f, 0.8f, 0.1f},

	//裏
	{1.0f, -1.0f, -1.0f, 0.8f, 0.1f, 0.8f},
	{-1.0f, -1.0f, -1.0f, 0.8f, 0.1f, 0.8f},
	{-1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.8f},
	{1.0f, -1.0f, -1.0f, 0.8f, 0.1f, 0.8f},
	{-1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.8f},
	{1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.8f},

	//下
	{-1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.8f},
	{1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.8f},
	{1.0f, -1.0f, 1.0f, 0.1f, 0.8f, 0.8f},
	{-1.0f, -1.0f, -1.0f, 0.1f, 0.8f, 0.8f},
	{1.0f, -1.0f, 1.0f, 0.1f, 0.8f, 0.8f},
	{-1.0f, -1.0f, 1.0f, 0.1f, 0.8f, 0.8f},

	//右
	{1.0f, -1.0f, 1.0f, 0.1f, 0.1f, 0.8f},
	{1.0f, -1.0f, -1.0f, 0.1f, 0.1f, 0.8f},
	{1.0f, 1.0f, -1.0f, 0.1f, 0.1f, 0.8f},
	{1.0f, -1.0f, 1.0f, 0.1f, 0.1f, 0.8f},
	{1.0f, 1.0f, -1.0f, 0.1f, 0.1f, 0.8f},
	{1.0f, 1.0f, 1.0f, 0.1f, 0.1f, 0.8f},
	
	//上
	{-1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.1f},
	{-1.0f, 1.0f, 1.0f, 0.8f, 0.1f, 0.1f},
	{1.0f, 1.0f, 1.0f, 0.8f, 0.1f, 0.1f},
	{-1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.1f},
	{1.0f, 1.0f, 1.0f, 0.8f, 0.1f, 0.1f},
	{1.0f, 1.0f, -1.0f, 0.8f, 0.1f, 0.1f},
	
	//前
	{-1.0f, -1.0f, 1.0f, 0.8f, 0.8f, 0.1f},
	{1.0f, -1.0f, 1.0f, 0.8f, 0.8f, 0.1f},
	{1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.1f},
	{-1.0f, -1.0f, 1.0f, 0.8f, 0.8f, 0.1f},
	{1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.1f},
	{-1.0f, 1.0f, 1.0f, 0.8f, 0.8f, 0.1f},
};

//六面体の面を塗りつぶす三角形の頂点のインデックス
//面ごとに独立する(同じ頂点番号を持たない)ように変更します
constexpr GLuint solidCubeIndex[] =
{
	0, 1, 2, 3, 4, 5, //左
	6, 7, 8, 9, 10, 11, //裏
	12, 13, 14, 15, 16, 17, //下
	18, 19, 20, 21, 22, 23, //右
	24, 25, 26, 27, 28, 29, //上
	30, 31, 32, 33, 34, 35, //前
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
	Window window;


	//GLFWwindow* const window(glfwCreateWindow(640, 480, "Hello", NULL, NULL));
	//if (window == NULL) {
	//	//ウィンドウが作成できなかった
	//	std::cerr << "Cannot create GLFW window." << std::endl;
	//	return 1;
	//}
	
	////作成したウィンドウをOpenGLの処理対象にする
	//glfwMakeContextCurrent(window);

	////GLEWを初期化する
	//glewExperimental = GL_TRUE;
	//if (glewInit() != GLEW_OK) {
	//	//GLEWの初期化に失敗した
	//	std::cerr << "Cannot initialize GLEW" << std::endl;
	//	return 1;
	//}

	////垂直同期のタイミングを待つ
	//glfwSwapInterval(1);

	//背景色を指定する
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

	//背面カリングを有効にする
	glFrontFace(GL_CCW); //頂点が左回りの面を表
	glCullFace(GL_BACK); //裏面を削除する
	glEnable(GL_CULL_FACE); //背面カリングを有効にする

	//デプスバッファを有効にする
	//デプスバッファに設定する深度
	glClearDepth(1.0);
	//奥行きの比較関数(ポリゴンの深度がデプスバッファより小さければ表示する)
	glDepthFunc(GL_LESS);
	//デプスバッファ法による陰面消去処理を有効
	glEnable(GL_DEPTH_TEST);
	
	////デバイス座標形状にビューポートを設定する
	//glViewport(100, 50, 300, 300);

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

	//uniform変数の場所を取得する
	//aspectは，オブジェクト名
	//const GLint aspectLoc(glGetUniformLocation(program, "aspect"));

	//uniform変数の場所を取得する
	//const GLint sizeLoc(glGetUniformLocation(program, "size"));
	//const GLint scaleLoc(glGetUniformLocation(program, "scale"));
	//const GLint locationLoc(glGetUniformLocation(program, "location"));

	//uniform変数の場所を取得する
	const GLint modelviewLoc(glGetUniformLocation(program, "modelview"));
	const GLint projectionLoc(glGetUniformLocation(program, "projection")); //投影変換行列をバーテックスシェーダに渡すため

	//図形データを作成する
	//１頂点あたりの要素数３，頂点数は１２
	std::unique_ptr<const Shape> shape(new SolidShapeIndex(3, 36, solidCubeVertex,36,solidCubeIndex));
	
	//タイマを0にセット
	glfwSetTime(0.0);

	//ウィンドウが開いている間繰り返す
	//glfwWindowShouldClose(window) == GL_FALSE
	//↓
	//window
	while (window)
	{
		//ウィンドウを消去する
		//GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BITとすることで，カラーバッファとデプスバッファを同時に塗りつぶすことができる
		//デプスバッファに設定する深度を指定
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//シェーダオブジェクトの使用開始(図形の描写に使用するプログラムオブジェクトを指定する)
		glUseProgram(program);

		//uniform変数に値を設定する
		//glUniform2fv(sizeLoc, 1, window.getSize());
		//glUniform1f(scaleLoc, window.getScale());
		//glUniform2fv(locationLoc, 1,window.getLocation());
		
		//uniform変数に値(float型の単一の変数)を設定する
		//glUniform1f(aspectLoc, window.getAspect());

		//透視投影変換行列を求める
		const GLfloat* const size(window.getSize());
		const GLfloat fovy(window.getScale() * 0.01f);
		//const GLfloat scale(window.getScale() * 2.0f);
		const GLfloat aspect(size[0] / size[1]);
		//const GLfloat w(size[0] / scale), h(size[1] / scale);
		const Matrix projection(Matrix::perspective(fovy, aspect, 1.0f, 10.0f));
		//const Matrix projection(Matrix::frustum(-w, w, -h, h, 1.0f, 10.0f));
		//const Matrix scaling(Matrix::scale(scale / size[0], scale / size[1], 1.0f));

		//平行移動の変換行列を求める
		//const GLfloat* const position(window.getLocation());
		//const Matrix traslation(Matrix::translate(position[0], position[1], 0.0f));

		//モデル変換行列を求める
		//const Matrix model(traslation * scaling);
		const GLfloat* const location(window.getLocation());
		const Matrix r(Matrix::rotate(static_cast<GLfloat>(glfwGetTime()),
			0.0f, 1.0f, 0.0f));
		const Matrix model(Matrix::translate(location[0], location[1], 0.0f) * r);

		//ビュー変換行列を求める
		//0.0f, 0.0f, 0.0fが，目標点(原点)
		const Matrix view(Matrix::lookat(3.0f, 4.0f, 5.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f));
		
		//モデルビュー変換行列を求める
		const Matrix modelview(view * model);

		//uniform変数に値を設定する
		//modelのメンバ配列matrixのポインタをdata()メソッドで取り出す
		//配列変数を転置して格納する
		glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection.data());
		glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, modelview.data());

		//図形を描画する
		shape->draw();

		//座標変換を一つ目の図形のモデルビュー変換の前に適応すると，二つ目の図形が一つ目の図形と一緒に動くようになる．
		//二つ目のモデルビュー変換行列を求める
		const Matrix modelview1(modelview * Matrix::translate(0.0f, 0.0f, 3.0f));

		//uniform変数に値を設定する
		glUniformMatrix4fv(modelviewLoc,1,GL_FALSE,modelview1.data());

		//二つ目の図形を描画する
		shape->draw();

		//カラーバッファを入れ替える
		//glfwSwapBuffers(window);
		//↓
		//window.swapBuffers();
		window.swapBuffers();

		//イベントを取り出す
		//glfwWaitEvents();
	}
}

