/* billboard-animated.fs
 * Fragment shader for sprite sheet effect (animated billboard)
 * Base code: Bryn Mawr College, alinen, 2020
 * Modified by JL
 * 29 March 2023
 */

#version 400

in vec2 uv;
in vec4 color;

uniform sampler2D image;
out vec4 FragColor;

void main() {
  vec4 c = color * texture(image, uv);
  FragColor = c;
}
