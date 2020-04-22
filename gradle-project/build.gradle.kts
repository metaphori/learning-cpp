plugins {
    //id("org.bitbucket.akornilov.cpp-build-tuner") version "0.7"
    id("loggersoft.cpp-ide-generator") version "0.5" apply false
}

allprojects{
    /*
    apply("cpp-ide-generator")
    configure<loggersoft.gradle.cpp.idegenerator.ConfigExtension> {
        // autoGenerate = true // Cannot access: it is private in 'ConfigExtension'
        // setAutoGenerate(true) // Does not seem to work
        setQtCreator(true)
    }
    */

    tasks.withType(CppCompile::class.java).configureEach {
        // Define a preprocessor macro for every binary
        // macros.put("NDEBUG", null)

        // Define a compiler options
        compilerArgs.add("-std=c++17")
        compilerArgs.add("-std=c++2a")

        // Define toolchain-specific compiler options
        /*
        compilerArgs.addAll(toolChain.map { toolChain ->
            when (toolChain) {
                is Gcc, is Clang -> listOf("-O2", "-fno-access-control")
                is VisualCpp -> listOf("/Zi")
                else -> listOf()
            }
        })
        */
    }
}


