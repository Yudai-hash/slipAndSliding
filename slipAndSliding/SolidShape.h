#pragma once

//図形の描画
#include "Shape.h"

//三角形を使った描画
//インデックスを使った図形の描画
class SolidShape
	:public Shape //Shapeクラスを継承
{
public:

	//コンストラクタ
	//size: 頂点の位置の次元
	//vertexcount: 頂点の数
	//vertex: 頂点属性を格納した配列
	SolidShape(GLint size, GLsizei vertexcount, const Object::Vertex *vertex)
		:Shape(size, vertexcount, vertex)
	{
	}

	//描画の実行
	//ShapeIndexクラスのexecute()メソッドのオーバーライドだけを行う．
	virtual void execute() const
	{
		//三角形で描画する
		//三角形を描画するので，GL_TRIANGLESを使用する
		glDrawArrays(GL_TRIANGLES, 0, vertexcount);
	}
};