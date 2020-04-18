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
}


