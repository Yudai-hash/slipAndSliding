/*座標変換は，バーテックスシェーダで行う*/
#version 150 core
/*layout修飾子で，attribute変数の場所やフラグメントシェーダの
出力変数の場所を宣言する機能を使用するため*/
/*#extension GL ARB explicit attrib location: enable*/
uniform mat4 modelview;
uniform mat4 projection;

/*attribute変数の場所やフラグメントシェーダの出力変数の場所を指定*/
/*layout (location = 0) in vec4 position;*/ 
/*layout (location = 1) in vec4 color;*/

/*size,scale,locationをmodelに統一する*/
/*modelの値は，一つの図形の描画中に変更されることはないので，
シェーダのuniform変数を用いる
描画する図形の頂点位置がバーテックスシェーダのin変数positionに
格納されている*/
/*uniform vec2 size;*/
/*uniform float scale;*/
/*uniform vec2 location;*/
in vec4 position;

/*頂点属性に追加した頂点色のデータを受け取るin変数colorを宣言する．*/
/*in変数は，シェーダプログラムがレンダリングパイプラインの前のステージデータを受け取る変数*/
in vec4 color;

/*線分にグラデーションをつけないように，
varying変数の宣言にflatという修飾子をつける*/
flat out vec4 vertex_color; 

void main()
{
vertex_color = color; /*out変数に格納された内容は，
レンダリングパイプラインの次のステージであるラスタライザに送られる．*/
/*projectioをモデルビュー変換後の頂点の座標値に乗じる*/
gl_Position = projection * modelview * position;
/*gl_Position = vec4(2.0 * scale / size, 1.0, 1.0) * position + vec4(location, 0.0, 0.0); positionを書かなかった場合，ウィンドウ上に表示されなかった*/
}