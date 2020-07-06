#version 120

// ユーティリティのインクルード
#pragma include "util.frag"
// 4D Simplex Noiseのインクルード
#pragma include "noise4D.frag"

uniform sampler2DRect u_posAndAgeTex;
uniform sampler2DRect u_velAndMaxAgeTex;
uniform float u_time;
uniform float u_timestep;
uniform float u_scale;

void main(void){
    vec2 st = gl_TexCoord[0].st;
    // 前の位置情報とパーティクル初期化からの経過時間を取得
    vec4 posAndAge = texture2DRect(u_posAndAgeTex,st);
    // 前の速度と生存期間を取得
    vec4 velAndMaxAge = texture2DRect(u_velAndMaxAgeTex,st);
    
    vec3 pos = posAndAge.xyz; // 前の位置
    vec3 vel = velAndMaxAge.xyz; // 前の速度
    
    float age = posAndAge.w; // 経過時間
    float maxAge = velAndMaxAge.w; // 生存期間

    age++;
    // Curl Noiseで速度を更新
    vel.x += 0.01*snoise(vec4(pos.x * u_scale, pos.y * u_scale, pos.z * u_scale, 0.1352 * u_time * u_timestep));
    vel.y += 0.01*snoise(vec4(pos.x * u_scale, pos.y * u_scale, pos.z * u_scale, 1.2814 * u_time * u_timestep));
    
    pos += vel;
    
    gl_FragData[0].rgba = vec4(pos, age); // 位置と経過時間を出力
    gl_FragData[1].rgba = vec4(vel, maxAge); //速度と生存期間を出力
}
