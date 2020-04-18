plugins {
    `cpp-application`
    `cpp-unit-test`
}

application {
    targetMachines.add(machines.linux.x86_64)

    dependencies {
        implementation(project(":proj-lib"))
    }
}
