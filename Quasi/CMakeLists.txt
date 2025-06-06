set(PROJECT_NAME Quasi)

set(HEADER_FILES
    src/Debug/internal_debug_break.h
    src/Debug/Logger.h
    src/Debug/Timer.h

    src/Graphics/GLs/IndexBuffer.h
    src/Graphics/GLs/VertexBuffer.h
    src/Graphics/GLs/Render.h
    src/Graphics/GLs/Shader.h
    src/Graphics/GLs/VertexArray.h
    src/Graphics/GLs/VertexBufferLayout.h
    src/Graphics/GLs/VertexElement.h
    src/Graphics/GLs/FrameBuffer.h
    src/Graphics/GLs/GLDebug.h
    src/Graphics/GLs/GLObject.h
    src/Graphics/GLs/GLTypeID.h
    src/Graphics/GLs/RenderBuffer.h
    src/Graphics/GLs/VertexBlueprint.h
    src/Graphics/GLs/TextureConstants.h
    src/Graphics/GLs/Texture.h

    src/Graphics/GraphicsDevice.h
    src/Graphics/Mesh.h
    src/Graphics/Mesh.tpp
    src/Graphics/RenderData.h
    src/Graphics/RenderObject.h
    src/Graphics/TriIndices.h
    src/Graphics/CameraController2D.h
    src/Graphics/CameraController3D.h
    src/Graphics/Light.h

    src/Graphics/ModelLoading/MTLMaterialLoader.h
    src/Graphics/ModelLoading/OBJModel.h
    src/Graphics/ModelLoading/OBJModelLoader.h
    src/Graphics/Fonts/Font.h
    src/Graphics/Fonts/FontDevice.h
    src/Graphics/Fonts/TextAlign.h
    src/Graphics/Fonts/TextRenderer.h
    src/Graphics/Fonts/TextStyle.h
    src/Graphics/Meshes/Circle.h
    src/Graphics/Meshes/MeshConstructor.h
    src/Graphics/Meshes/Cube.h
    src/Graphics/Meshes/CubeNormless.h
    src/Graphics/Meshes/Sphere.h
    src/Graphics/Meshes/Icosphere.h
    src/Graphics/Meshes/Quad.h
    src/Graphics/Meshes/Plane.h
    src/Graphics/Meshes/Capsule.h
    src/Graphics/Meshes/Stadium.h
    src/Graphics/GUI/ImGuiExt.h

    src/IO/IO.h
    src/IO/Keyboard.h
    src/IO/Mouse.h
    src/IO/TimeType.h

    src/Math/Constants.h
    src/Math/Matrix.h
    src/Math/Rect.h
    src/Math/Vector.h
    src/Math/Geometry.h
    src/Math/Angles.h
    src/Math/Random.h
    src/Math/Color.h
    src/Math/Complex.h
    src/Math/Quaternion.h
    src/Math/Transform2D.h
    src/Math/Transform3D.h

    src/Physics/World2D.h
    src/Physics/Shape2D.h
    src/Physics/Body2D.h
    src/Physics/Collision2D.h
    src/Physics/PhysicsTransform2D.h
    src/Physics/Manifold2D.h
    src/Physics/SeperatingAxisSolver.h
    src/Physics/CircleShape2D.h
    src/Physics/CapsuleShape2D.h
    src/Physics/PolygonShape2D.h
    src/Physics/RectShape2D.h

    src/Utils/Enum.h
    src/Utils/Text.h
    src/Utils/Option.h
    src/Utils/Macros.h
    src/Utils/RichString.h
    src/Utils/Type.h
    src/Utils/Match.h
    src/Utils/Memory.h
    src/Utils/Iterator.h
    src/Utils/Vec.h
    src/Utils/Span.h
    src/Utils/Algorithm.h
    src/Utils/Array.h
    src/Utils/Comparison.h
    src/Utils/Box.h
    src/Utils/ArrayBox.h
    src/Utils/Str.h
    src/Utils/String.h
    src/Utils/CStr.h
    src/Utils/Numeric.h
    src/Utils/Tuple.h
    src/Utils/Bitwise.h
    src/Utils/Hash.h
    src/Utils/HashMap.h
    src/Utils/Range.h
    src/Utils/MacroIteration.h
    src/Utils/Text/Parsing.h
    src/Utils/Text/Num.h
    src/Utils/Text/StringWriter.h
    src/Utils/Text/Formatting.h
    src/Utils/Iter/MapIter.h
    src/Utils/Iter/EnumerateIter.h
    src/Utils/Iter/LinesIter.h
    src/Utils/Iter/SplitIter.h

    src/vendor/imgui/imconfig.h
    src/vendor/imgui/imgui.h
    src/vendor/imgui/imgui_impl_glfw.h
    src/vendor/imgui/imgui_impl_opengl3.h
    src/vendor/imgui/imgui_internal.h
    src/vendor/imgui/imstb_rectpack.h
    src/vendor/imgui/imstb_textedit.h
    src/vendor/imgui/imstb_truetype.h
    src/vendor/imgui/imgui_stdlib.h

    src/vendor/stb_image/stb_image.h
)
source_group("Header Files" FILES ${HEADER_FILES})

set(SOURCE_FILES
    src/Debug/Logger.cpp
    src/Debug/Timer.cpp

    src/Graphics/GLs/FrameBuffer.cpp
    src/Graphics/GLs/GLDebug.cpp
    src/Graphics/GLs/RenderBuffer.cpp
    src/Graphics/GLs/IndexBuffer.cpp
    src/Graphics/GLs/VertexBuffer.cpp
    src/Graphics/GLs/VertexArray.cpp
    src/Graphics/GLs/VertexBufferLayout.cpp
    src/Graphics/GLs/Render.cpp
    src/Graphics/GLs/Shader.cpp
    src/Graphics/GLs/Texture.cpp

    src/Graphics/CameraController2D.cpp
    src/Graphics/CameraController3D.cpp
    src/Graphics/Light.cpp
    src/Graphics/GraphicsDevice.cpp
    src/Graphics/RenderData.cpp

    src/Graphics/ModelLoading/MTLMaterialLoader.cpp
    src/Graphics/ModelLoading/OBJModelLoader.cpp
    src/Graphics/ModelLoading/OBJModel.cpp
    src/Graphics/Fonts/Font.cpp
    src/Graphics/Fonts/FontDevice.cpp
    src/Graphics/Fonts/TextAlign.cpp
    src/Graphics/Fonts/TextRenderer.cpp
    src/Graphics/GUI/ImGuiExt.cpp

    src/IO/IO.cpp
    src/IO/Keyboard.cpp
    src/IO/Mouse.cpp
    src/IO/TimeType.cpp

    src/Math/Vector.cpp
    src/Math/Matrix.cpp
    src/Math/Rect.cpp
    src/Math/Color.cpp
    src/Math/Complex.cpp
    src/Math/Quaternion.cpp
    src/Math/Transform2D.cpp
    src/Math/Transform3D.cpp

    src/Physics/World2D.cpp
    src/Physics/Shape2D.cpp
    src/Physics/Body2D.cpp
    src/Physics/Collision2D.cpp
    src/Physics/PhysicsTransform2D.cpp
    src/Physics/Manifold2D.cpp
    src/Physics/SeperatingAxisSolver.cpp
    src/Physics/CircleShape2D.cpp
    src/Physics/CapsuleShape2D.cpp
    src/Physics/PolygonShape2D.cpp
    src/Physics/RectShape2D.cpp

    src/Utils/RichString.cpp
    src/Utils/Text.cpp
    src/Utils/Str.cpp
    src/Utils/String.cpp
    src/Utils/CStr.cpp
    src/Utils/Memory.cpp
    src/Utils/Bitwise.cpp
    src/Utils/Hash.cpp
    src/Utils/Range.cpp
    src/Utils/Text/Parsing.cpp
    src/Utils/Text/Num.cpp
    src/Utils/Text/StringWriter.cpp
    src/Utils/Text/Formatting.cpp
    src/Utils/Iter/LinesIter.cpp

    src/vendor/imgui/imgui.cpp
    src/vendor/imgui/imgui_demo.cpp
    src/vendor/imgui/imgui_draw.cpp
    src/vendor/imgui/imgui_impl_glfw.cpp
    src/vendor/imgui/imgui_impl_opengl3.cpp
    src/vendor/imgui/imgui_tables.cpp
    src/vendor/imgui/imgui_widgets.cpp
    src/vendor/imgui/imgui_stdlib.cpp

    src/vendor/stb_image/stb_image.cpp
)
source_group("Source Files" FILES ${SOURCE_FILES})

set(ALL_FILES
    ${HEADER_FILES}
    ${SOURCE_FILES}
)

add_library(${PROJECT_NAME} STATIC ${ALL_FILES})

target_include_directories(${PROJECT_NAME}
        PRIVATE
    ${PROJECT_SOURCE_DIR}/Dependencies/GLFW/include/
    # /Library/Developer/CommandLineTools/SDKs/MacOSX13.3.sdk/System/Library/Frameworks/
        PUBLIC
    src/
    src/Graphics/
    src/vendor/imgui/
    src/vendor/
    src/vendor/freetype/
    ${PROJECT_SOURCE_DIR}/OpenGLPort/
)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    Q_EXT_MATCH_SYNTAX # for cool syntax features for pattern matching
)

target_link_libraries(${PROJECT_NAME} PUBLIC
    OpenGLPort
    # opengl32.dll
    ${CMAKE_SOURCE_DIR}/Dependencies/GLFW/lib-mingw-w64/libglfw3.a
    ${CMAKE_CURRENT_SOURCE_DIR}/src/vendor/freetype/libfreetype.a
)
# execute_process()