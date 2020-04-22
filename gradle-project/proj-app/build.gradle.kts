plugins {
    `cpp-application`
}

application {
    targetMachines.add(machines.linux.x86_64)

    dependencies {
        implementation(project(":proj-lib"))
    }
}
