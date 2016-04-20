#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

#include "c_semver.h"


static int32_t semver_check(const char *semver) {
	struct semver_context s;
	semver_init(&s, semver);
	printf("parsing %s\n", semver);
	int32_t ret = semver_parse(&s);
	printf(
		"  -> return %d, parsed %d.%d.%d, prerelease=%d, metadata=%d, prerelease_size=%d, metadata_size=%d\n",
		ret,
		s.major,
		s.minor,
		s.patch,
		s.is_prerelease,
		s.has_metadata,
		(int)s.prerelease_size,
		(int)s.metadata_size
	);
	semver_free(&s);
	return ret;
}


int main(void) {
	/* Check invalid inputs. */
	assert(semver_check("") != SEMVER_PARSE_OK);
	assert(semver_check("0") != SEMVER_PARSE_OK);
	assert(semver_check("12") != SEMVER_PARSE_OK);
	assert(semver_check("abc") != SEMVER_PARSE_OK);
	assert(semver_check("-") != SEMVER_PARSE_OK);
	assert(semver_check("+") != SEMVER_PARSE_OK);
	assert(semver_check(".") != SEMVER_PARSE_OK);
	assert(semver_check("0.") != SEMVER_PARSE_OK);
	assert(semver_check("0.1") != SEMVER_PARSE_OK);
	assert(semver_check(".0.1") != SEMVER_PARSE_OK);
	assert(semver_check("sdlfbhsdjfhbsdbsol") != SEMVER_PARSE_OK);

	/* Check valid versions. */
	assert(semver_check("0.0.0") == SEMVER_PARSE_OK);
	assert(semver_check("0.1.0") == SEMVER_PARSE_OK);
	assert(semver_check("52.68.123") == SEMVER_PARSE_OK);

	/* Invalid versions (leading zero). */
	assert(semver_check("052.68.123") != SEMVER_PARSE_OK);
	assert(semver_check("52.068.123") != SEMVER_PARSE_OK);
	assert(semver_check("52.68.0123") != SEMVER_PARSE_OK);

	/* Valid prerelease versions with numerical and alphanumerical identifiers. */
	assert(semver_check("1.2.3-15") == SEMVER_PARSE_OK);
	assert(semver_check("1.2.3-abc") == SEMVER_PARSE_OK);
	assert(semver_check("1.2.3-a.1") == SEMVER_PARSE_OK);
	assert(semver_check("1.2.3-a.1.b.4") == SEMVER_PARSE_OK);
	assert(semver_check("1.2.3-a.1.b.54") == SEMVER_PARSE_OK);
	assert(semver_check("1.2.3-a.1.b.c-df.15") == SEMVER_PARSE_OK);

	/* Check invalid prerelease versions (leading zero). */
	assert(semver_check("1.2.3-068") != SEMVER_PARSE_OK);
	assert(semver_check("1.2.3-a.1.b.054") != SEMVER_PARSE_OK);

	/* Check versions with valid prerelease and metadata fields. */
	assert(semver_check("2.3.4+20160404") == SEMVER_PARSE_OK);
	assert(semver_check("2.3.4+abcdef") == SEMVER_PARSE_OK);
	assert(semver_check("2.3.4+abcdef.16.5") == SEMVER_PARSE_OK);
	assert(semver_check("1.2.3-a.1.b.c-df.15") == SEMVER_PARSE_OK);
	assert(semver_check("3.4.5-a.meh.14") == SEMVER_PARSE_OK);
	assert(semver_check("3.4.5-a.1-meh.2+20160404") == SEMVER_PARSE_OK);

	/* Metadata with a leading zero numeric identifier. */
	assert(semver_check("2.3.4+00155885") != SEMVER_PARSE_OK);

	/* Invalid version with two metadata fields. */
	assert(semver_check("2.3.4+abcdef.16.5+meh") != SEMVER_PARSE_OK);

	/* Check very long inuts. */
	assert(semver_check("loooooooooooooooooooooooooooooooooooooooooooooooooooong") != SEMVER_PARSE_OK);
	assert(semver_check("1.2.3-loooooooooooooooooooooooooooooooooooooooooooooooooooong") == SEMVER_PARSE_OK);
	assert(semver_check("1.2.3+loooooooooooooooooooooooooooooooooooooooooooooooooooong") == SEMVER_PARSE_OK);

	printf("all tests passed\n");
	return 0;
}
