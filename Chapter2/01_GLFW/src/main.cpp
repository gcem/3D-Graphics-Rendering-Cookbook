#include <glad/glad.h>
// keep glad.h on top
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

// the vscode shader extension highlights the shader code in raw strings but
// the literal should have glsl as delimiter

// VERTEX SHADER
static const char *shaderCodeVertex = R"glsl(
#version 460 core
layout (location=0) out vec3 color;
const vec2 pos[3] = vec2[3](
	vec2(-0.6, -0.4),
	vec2( 0.6, -0.4),
	vec2( 0.0,  0.6)
);
const vec3 col[3] = vec3[3](
	vec3( 1.0, 0.0, 0.0 ),
	vec3( 0.0, 1.0, 0.0 ),
	vec3( 0.0, 0.0, 1.0 )
);
void main()
{
	gl_Position = vec4(pos[gl_VertexID], 0.0, 1.0);
	color = col[gl_VertexID];
}
)glsl";

// FRAGMENT SHADER
static const char *shaderCodeFragment = R"glsl(
#version 460 core
layout (location=0) in vec3 color;
layout (location=0) out vec4 out_FragColor;
void main()
{
	out_FragColor = vec4(color, 1.0);
};
)glsl";

// END OF SHADERS

int
main(void)
{
    glfwSetErrorCallback([](int error, const char *description) {
        fprintf(stderr, "Error: %s\n", description);
    });

    if (!glfwInit())
        exit(EXIT_FAILURE);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window =
        glfwCreateWindow(1024, 768, "Simple example", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(
        window,
        [](GLFWwindow *window, int key, int scancode, int action, int mods) {
            if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
                glfwSetWindowShouldClose(window, GLFW_TRUE);
        });

    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSwapInterval(1);

    const unsigned char *vendor = glGetString(GL_VENDOR);
    const unsigned char *renderer = glGetString(GL_RENDERER);
    printf("Vendor: %s\nRenderer: %s\n", vendor, renderer);

    const GLuint shaderVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderVertex, 1, &shaderCodeVertex, nullptr);
    glCompileShader(shaderVertex);

    const GLuint shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderFragment, 1, &shaderCodeFragment, nullptr);
    glCompileShader(shaderFragment);

    const GLuint program = glCreateProgram();
    glAttachShader(program, shaderVertex);
    glAttachShader(program, shaderFragment);

    glLinkProgram(program);
    glUseProgram(program);

    GLuint vao;
    glCreateVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glfwSetFramebufferSizeCallback(
        window, [](GLFWwindow *window, int width, int height) {
            // TODO does this need synchronization with the rendering loop?
            glViewport(0, 0, width, height);
        });

    while (!glfwWindowShouldClose(window)) {
        // int width, height;
        // glfwGetFramebufferSize(window, &width, &height);
        // glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteProgram(program);
    glDeleteShader(shaderFragment);
    glDeleteShader(shaderVertex);
    glDeleteVertexArrays(1, &vao);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
