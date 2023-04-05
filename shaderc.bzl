load("@bazel_skylib//lib:paths.bzl", "paths")

load("@//:configuration.bzl", "COPT_CXX")
load("@rules_cc//cc:defs.bzl", "cc_binary")

# FIXME: source files (src, varying_def) must be written with '//*:*' notation,
#        this is cumbersome and concat'ing it with '$(location ...)' is very ugly.
#        find way to make this behave like normal 'srcs=[...]' attributes?
# FIXME: bash command not compatible with windows; rewrite for powershell too?
def shaderc_bin_file(name, src, type, varying_def, includes=None, **kwargs):
    # FIXME: add attribute for choosing own compiler? avoid dep on @bgfx.
    compiler = "$(location @bgfx//:shaderc)"
    outfile = name + '.bin.h'
    srcs = [src]

    varying_def_args = ['--varyingdef']
    varying_def_args.append('$(location ' + varying_def + ')')
    srcs.append(varying_def)

    includes_args = ['-i', "$(location @bgfx//:shader-include)"]
    srcs.append('@bgfx//:shader-include')
    if includes:
        for include in includes:
            srcs.append(include)
            includes_args += ['-i', '$(location ' + include + ')']

    def gencmd(platform, profile, ident):
        profile_args = []
        if profile:  profile_args = ['-p', profile]

        TMP_OUT = 'tmp_' + ident
        optional_args = profile_args + varying_def_args + includes_args
        return ' '.join([
            compiler, '-f', '$(location ' + src + ')', '-o', TMP_OUT,
            '--type', type,
            '--platform', platform,
        ] + optional_args  + [
            '--bin2c', name + '_' + ident,
            '&&', 'cat', TMP_OUT, '>>', "$(RULEDIR)/" + outfile,
            '&&', 'echo', '>>', "$(RULEDIR)/" + outfile,
        ])

    cmds = [
        gencmd('linux',     '120', 'glsl'),
        gencmd('android',    None, 'essl'),
        gencmd('linux',   'spirv',  'spv'),
        # FIXME: conditionally compile DirectX shaders for Windows only
        #gencmd('windows', 's_3_0',  'dx9'),
        #gencmd('windows', 's_5_0', 'dx11'),
        gencmd('ios',     'metal',  'mtl'),
        "echo \"extern const uint8_t* " + name + "_pssl;\"      >> $(RULEDIR)/" + outfile,
        "echo \"extern const uint32_t " + name + "_pssl_size;\" >> $(RULEDIR)/" + outfile,
    ]
    cmds = [
        "echo \"/* shaderc.bzl: generated compiled shaders with cmd: */\" > $(RULEDIR)/" + outfile,
        "cat >> $(RULEDIR)/" + outfile + " <<-EOF\n/*\n    " + ' && '.join(cmds) + "\n*/\n\nEOF\ntrue",
    ] + cmds
    cmd = ' && '.join(cmds)
    native.genrule(
        name = name,
        srcs = srcs,
        outs = [outfile],
        cmd = cmd,
        tools = ["@bgfx//:shaderc", "@bgfx//:shader-include", src],
        **kwargs
    )

shader_types = {
    'vs': 'vertex',
    'fs': 'fragment',
    'cs': 'compute',
}

# compile bgfx shaders provided in `srcs`, produces a cc_library of headers
# filled with binary data.
# `name` can be anything, each source is transformed to a header file as per:
#    //example:path/vs_example.sc  ->  //example:vs_example.bin.h
# FIXME: preserve preceding paths from srcs, don't just dump into $(RULEDIR).
def shaderc_bin(name, srcs, varying_def, includes=None, **kwargs):
    hdrs = []
    for src in srcs:
        # FIXME: very ugly stripping of preceding '//xxx:', again
        # find way to cause label expansion before ever getting here?
        bname = src.split(':')[-1].removesuffix('.sc')
        bname = paths.basename(bname)
        # FIXME: bizzare warning about `bname` being a directory? not true.
        shaderc_bin_file(
            name = bname,
            src = src,
            type = shader_types[bname[:2]],
            includes = includes,
            varying_def = varying_def,
        )
        hdrs.append(':' + bname)

    native.cc_library(
        name = name,
        hdrs = hdrs,
        visibility = ['//visibility:public'],
        **kwargs
    )

