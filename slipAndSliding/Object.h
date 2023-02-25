#pragma once //同じヘッダファイルが複数回読み込まれないようにするおまじない
#include <GL/glew.h>

//クラスObject
//図形データ
class Object
{
	//メンバ変数 vaoとvbo
	//頂点配列オブジェクト名
	GLuint vao;

	//頂点バッファオブジェクト名
	GLuint vbo;

	//publicメンバ
	public:
		//頂点属性
		struct Vertex
		{
			//位置
			GLfloat position[2];
		};
		
		//コンストラクタ
		//size: 頂点の位置の次元
		//vertexcount: 頂点の数
		//vertex: 頂点属性を格納した配列
		Object(GLint size, GLsizei vertexcount, const Vertex* vertex)
		{
			//頂点配列オブジェクト
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			//頂点バッファオブジェクト
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER,
			vertexcount * sizeof(Vertex), vertex, GL_STATIC_DRAW);

			//結合されている頂点バッファオブジェクトをin変数から参照できるようにする
			glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(0);
		}

	//デストラクタ
		//仮想関数
	virtual ~Object()
	{
		//頂点配列オブジェクトを削減する
		glDeleteVertexArrays(1, &vao);

		//頂点バッファオブジェクトを削除する
		glDeleteBuffers(1, &vbo);
	}

	//コピーコンストラクタと代入演算子をprovateメンバにして，インスタンスのコピーを禁止します．(コピーしようとすると，コンパイルエラーになる)
	private:
		//コピーコンストラクタによるコピー禁止
		Object(const Object& o);
		
		//代入によるコピー禁止
		Object& operator=(const Object& o);
	
    public:
		//頂点配列オブジェクトの結合
		//インスタンスを変更しないので，constメソッド
		void bind() const
		{
			//描写する頂点配列オブジェクトを指定する
		    glBindVertexArray(vao);
	     }
};




	