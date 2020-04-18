plugins {
    `cpp-library`
    // `cpp-unit-test`
}

library {
    linkage.set(listOf(Linkage.STATIC)) // Linkage.SHARED
}
