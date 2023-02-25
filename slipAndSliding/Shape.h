//#をプリプロセッサ＝コンパイラが文法解釈する前に実行される
#pragma once
#include <memory> // 環境変数などで設定されたパスから探す

//図形データ
#include "Object.h" //まずは，カレントディレクトリから探す

//図形の描写
class Shape
{
	//図形データ
	std::shared_ptr<const Object> object;

//vertexcountメンバは，派生クラスからも参照するので，protectedとする
protected:
	//描写に使う頂点の数(変更することがないので，const)
	const GLsizei vertexcount;

public:
	//コンストラクタ
	//size: 頂点の位置の次元
	//vertexcount: 頂点の数
	//vertex: 頂点属性を格納した配列
	Shape(GLint size, GLsizei vertexcount, const Object::Vertex * vertex):
		object(new Object(size,vertexcount,vertex)) //引数で受け取った頂点属性を使ってObjectクラスのインスタンスを生成し，それによりobjectを初期化する
		,vertexcount(vertexcount)
	{
	}

	//描画
	void draw() const
	{
		//頂点配列オブジェクトを結合する
		object->bind();
		
		//描画を実行する
		execute();
	}

	//描画の実行
	//execute()は，仮想関数にして，このクラス(Shapeクラス)から派生したクラスでオーバーライドできるようにする
	virtual void execute() const
	{
		//折れ線で描画する
		//頂点バッファオブジェクトの先頭の頂点から描画するので，0
		glDrawArrays(GL_LINE_LOOP, 0, vertexcount);
	}
};


