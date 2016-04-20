# Pure C SemVer version string parser and checker

This is a simple and pure-C library to check and parse version strings conforming
to the Semantic Versioning 2.0.0 standard. Of course there are many ways of doing
this, but this piece of code has some different main goals - it is targeting embedded
environments. It has very low stack and memory requirements and the code size and
complexity is low to be usable even on 8bit microcontrollers.

Usage:

``` c
struct semver_context s;
semver_init(&s, "1.2.3-alpha.1+20160420");
semver_parse(&s);
printf("major = %d, minor = %d, patch = %d\n", s.major, s.minor, s.patch);
semver_free(&s);

```
