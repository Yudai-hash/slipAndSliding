#pragma once

//インデックスを使った図形の描画
#include "ShapeIndex.h"

//インデックスを使った図形の描画
class SolidShapeIndex
	:public ShapeIndex //ShapeIndexクラスを継承
{
public:

	//コンストラクタ
	//size: 頂点の位置の次元
	//vertexcount: 頂点の数
	//vertex: 頂点属性を格納した配列
	//indexcount: 頂点のインデックスの要素数
	//index: 頂点のインデックスを格納した配列
	SolidShapeIndex(GLint size, GLsizei vertexcount, const Object::Vertex *vertex,
		GLsizei indexcount, const GLuint* index)
		:ShapeIndex(size, vertexcount, vertex, indexcount, index)
	{
	}

	//描画の実行
	//ShapeIndexクラスのexecute(9メソッドのオーバーライドだけを行う．
	virtual void execute() const
	{
		//三角形を描画する
		glDrawElements(GL_TRIANGLES, indexcount, GL_UNSIGNED_INT, 0);
	}
};