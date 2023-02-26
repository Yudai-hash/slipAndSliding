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

	//インデックスの頂点バッファオブジェクト(privateメンバ)
	GLuint ibo;

	//publicメンバ
	public:
		//頂点属性
		struct Vertex
		{
			//位置
			//頂点の位置が三次元なので，Objectクラスの中で定義しているVertex構造体のメンバpositionの要素数を3にする
			GLfloat position[3];

			//色
			GLfloat color[3];
		};
		
		//コンストラクタ
		//size: 頂点の位置の次元
		//vertexcount: 頂点の数
		//vertex: 頂点属性を格納した配列
		//indexcount: 頂点のインデックスの要素数
		//index: 頂点のインデックスを格納した配列
		Object(GLint size, GLsizei vertexcount, const Vertex *vertex,
			GLsizei indexcount = 0,const GLuint *index = NULL)
		{
			//頂点配列オブジェクト
			glGenVertexArrays(1, &vao);
			glBindVertexArray(vao);

			//頂点バッファオブジェクト
			glGenBuffers(1, &vbo);
			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			//indexcount = 0，vertex = NULLでもエラーにはならない
			glBufferData(GL_ARRAY_BUFFER,
			vertexcount * sizeof(Vertex), vertex, GL_STATIC_DRAW);

			//インデックスの頂点バッファオブジェクト
			glGenBuffers(1, &ibo);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				indexcount * sizeof(GLuint), index, GL_STATIC_DRAW);

			//結合されている頂点バッファオブジェクトをin変数から参照できるようにする
			//static_cast<Vertex *>(0)->positionは，Vertex型の先頭にあるので，実質的に0
			glVertexAttribPointer(0, size, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				static_cast<Vertex*>(0)->position);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				static_cast<Vertex*>(0)->color);
			glEnableVertexAttribArray(1);
		}

	//デストラクタ
		//仮想関数
	virtual ~Object()
	{
		//頂点配列オブジェクトを削減する
		glDeleteVertexArrays(1, &vao);

		//頂点バッファオブジェクトを削除する
		glDeleteBuffers(1, &vbo);

		//インデックスの頂点バッファオブジェクトを削除する
		glDeleteBuffers(1, &ibo);
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




	