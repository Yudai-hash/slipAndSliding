#version 150 core
/*layout修飾子で，attribute変数の場所やフラグメントシェーダの出力変数の場所を宣言する機能を使用するため*/
/*#extension GL ARB explicit attrib location: enable*/

flat in vec4 vertex_color; /*バーテックスシェーダからラスタライザに送られ，それに補間された頂点色を受け取るin変数vertex_colorを宣言*/
/*attribute変数の場所やフラグメントシェーダの出力変数の場所を指定*/
/*layout (location = 0) out vec4 fragment;*/
out vec4 fragment;

void main()
{
fragment = vertex_color; /*out変数fragmentに代入して，フラグメントシェーダの次のステージであるフレームバッファ*/ 
}