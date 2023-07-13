#pragma once

"#ifdef VERTEX\n"
"layout(location=0) in vec2 Position;"
"layout(location=1) in vec2 TexCoord;"
"layout(location=2) in vec4 Color1;"
"layout(location=3) in vec4 Color2;"
"layout(location=4) in ivec2 TexIds;"
"layout(location=5) in uvec4 Flags;"
"layout(location=6) in vec2 Extra;"
"uniform mat4 u_MVP;"
"out vec4 v_Position;"
"out vec2 v_TexCoord;"
"out vec4 v_Color1,v_Color2;"
"flat out ivec2 v_TexIds;"
"flat out uvec4 v_Flags;"
"out vec2 v_Extra;"
"void main()"
"{"
  "v_Position=u_MVP*vec4(Position,0,1);"
  "gl_Position=v_Position;"
  "v_TexCoord=TexCoord;"
  "v_Color1=Color1.wzyx;"
  "v_Color2=Color2.wzyx;"
  "v_TexIds=TexIds;"
  "v_Flags=Flags;"
  "v_Extra=Extra;"
"}"
"\n#elif FRAGMENT\n"
"layout(location=0) out vec4 FragColor;"
"uniform sampler2D u_MapTexture;"
"uniform sampler2DArray u_CursorTexture,u_FontTexture;"
"uniform sampler2D u_MaskTexture;"
"in vec4 v_Position;"
"in vec2 v_TexCoord;"
"in vec4 v_Color1,v_Color2;"
"flat in ivec2 v_TexIds;"
"flat in uvec4 v_Flags;"
"in vec2 v_Extra;"
"uniform vec2 u_Scale;"
"uniform vec4 u_TextMask;"
"uniform bool u_IsMasking=false;"
"uniform vec4 u_Viewport;"
"float v(vec3 v,float u,float y)"
"{"
  "float r=max(min(v.x,v.y),min(max(v.x,v.y),v.z)),n=max(u,1.)*(r-1.+.5*y);"
  "return clamp(n+.5,0.,1.);"
"}"
"vec3 v(vec3 v,float u)"
"{"
  "float y=dot(v,vec3(.299,.587,.114));"
  "return mix(v,vec3(y),u);"
"}"
"void main()"
"{"
  "switch(v_Flags.x){"
    "case 1u:"
      "FragColor=texture(u_CursorTexture,vec3(v_TexCoord,v_TexIds.x))*v_Color1;"
      "break;"
    "case 2u:"
      "FragColor=v_Color1;"
      "break;"
    "case 3u:"
      "{"
        "vec4 d=texture(u_FontTexture,vec3(v_TexCoord,v_TexIds.x));"
        "if(v_Flags.y>0u)"
          "{"
            "float r=v_Flags.y==1u?"
              "1.:"
              ".5;"
            "FragColor=vec4(v_Color1.x+v_Color1.y+v_Color1.z<.1?"
              ".8:"
              "0.);"
            "FragColor.w=(d.w<.26?"
              "0.:"
              "smoothstep(0.,1.,d.w))*r*v_Extra.x;"
          "}"
        "else "
           "if(v_Flags.w==1u)"
            "{"
              "float y=v(d.xyz,v_Extra.x,v_Extra.y+.02),r=v(d.xyz,v_Extra.x,1.01);"
              "FragColor=vec4(mix(v_Color2.xyz,v_Color1.xyz,r),v_Color1.w*y);"
            "}"
          "else "
            "{"
              "float y=smoothstep(1.5,1.,u_Scale.x),r=v(d.xyz,v_Extra.x/(1.+y),v_Extra.y);"
              "FragColor=vec4(v_Color1.xyz,v_Color1.w*r*(1.+.2*y));"
            "}"
        "if(u_IsMasking&&v_Flags.z==0u)"
          "if(u_TextMask.x<v_Position.x&&u_TextMask.z>v_Position.x&&u_TextMask.y>v_Position.y&&u_TextMask.w<v_Position.y)"
            "FragColor.w=0.;"
          "else "
             "FragColor.w*=.7;"
      "}"
      "break;"
    "case 4u:"
      "FragColor=vec4(0);"
      "break;"
    "case 5u:"
      "FragColor=texture(u_MapTexture,v_TexCoord);"
      "FragColor.xyz=v(FragColor.xyz,.2);"
      "if(v_Flags.z>0u)"
        "FragColor.w*=v_Flags.z/1e2;"
      "break;"
    "case 6u:"
      "FragColor=v_Color1;"
      "float r=v_TexCoord.x<.5?"
        "smoothstep(0.,v_Extra.x,v_TexCoord.x):"
        "smoothstep(1.,v_Extra.y,v_TexCoord.x);"
      "FragColor.w*=r;"
      "break;"
    "case 7u:"
      "FragColor.xyz=v_Color2.xyz*(smoothstep(0.,2.,v_TexCoord.y)/1.4);"
      "FragColor.w=v_Color1.w;"
      "break;"
  "}"
  "if(v_Flags.x!=3u)"
    "{"
      "float r=1.00001;"
      "vec2 d=vec2(r/u_Scale.x/v_Extra.x,r/u_Scale.y/v_Extra.y),n=d*2.,s=d*3.;"
      "switch(v_Flags.y){"
        "case 1u:"
          "if(v_TexCoord.x<d.x||v_TexCoord.x>1.-d.x||v_TexCoord.y<d.y||v_TexCoord.y>1.-d.y)"
            "FragColor=v_Color2;"
        "case 2u:"
          "if(v_TexCoord.x>n.x&&v_TexCoord.x<s.x||v_TexCoord.x<1.-n.x&&v_TexCoord.x>1.-s.x||v_TexCoord.y>n.y&&v_TexCoord.y<s.y||v_TexCoord.y<1.-n.y&&v_TexCoord.y>1.-s.y)"
            "FragColor=v_Color2;"
          "break;"
        "case 3u:"
          "if(v_TexCoord.x<s.x||v_TexCoord.x>=1.-s.x||v_TexCoord.y<s.y||v_TexCoord.y>=1.-s.y)"
            "FragColor=v_Color1;"
          "if(v_TexCoord.x<d.x||v_TexCoord.x>=1.-d.x||v_TexCoord.y<d.y||v_TexCoord.y>=1.-d.y)"
            "FragColor=v_Color2;"
          "if(v_TexCoord.x>=n.x&&v_TexCoord.x<s.x||v_TexCoord.x<1.-n.x&&v_TexCoord.x>=1.-s.x||v_TexCoord.y>=n.y&&v_TexCoord.y<s.y||v_TexCoord.y<1.-n.y&&v_TexCoord.y>=1.-s.y)"
            "FragColor=v_Color2;"
          "break;"
        "case 4u:"
          "if(v_TexCoord.x<d.x||v_TexCoord.x>1.-d.x||v_TexCoord.y<d.y||v_TexCoord.y>1.-d.y)"
            "FragColor=v_Color2;"
          "break;"
      "}"
      "if(v_Flags.z>0u)"
        "FragColor.w*=float(v_Flags.z)*.01;"
    "}"
  "if(texture(u_MaskTexture,(gl_FragCoord.xy-u_Viewport.xy)/u_Viewport.zw).x>.1)"
    "FragColor.w=0.;"
"}"
"\n#endif"