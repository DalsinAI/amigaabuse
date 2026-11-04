/* Abuse's Lisp engine evaluates and collects garbage by recursion, and
 * a Shell's 4 KB stack is far too little; libnix's startup sets up this
 * much instead.
 * The AmiCradle Team. Public domain, like Abuse's own sources. */
unsigned long __stack = 512 * 1024;
