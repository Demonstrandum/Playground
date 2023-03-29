# use `wayland-scanner` to generate files for wayland support

# FIXME: more robust way of finding the `wayland-scanner` binary
def _gen_variables(protocols):
    srcs = []
    outs = []
    cmds = []
    for obj in protocols:
        client_header_out = "{}.h".format(obj['stem'])
        private_code_out  = "{}-code.h".format(obj['stem'])
        src = obj['base'] + '/' + obj['name']
        srcs += [src]
        outs += [client_header_out, private_code_out]
        cmd = "wayland-scanner {} {} $(RULEDIR)/{}"
        cmds += [cmd.format('client-header', src, client_header_out)]
        cmds += [cmd.format('private-code',  src, private_code_out)]

    return srcs, outs, cmds

def _wayland_generate_protocols(name, protocols, **kwargs):
    srcs, outs, cmds = _gen_variables(protocols)

    native.genrule(
        name = name,
        #srcs = srcs,
        outs = outs,
        cmd = ' && '.join(cmds),
        **kwargs
    )

def wayland_generate_protocols(name, protocols, **kwargs):
    _, outs, _ = _gen_variables(protocols)
    _wayland_generate_protocols(
        name = '_' + name,
        protocols = protocols,
    )

    native.cc_library(
        name = name,
        hdrs = outs,
        include_prefix = '.',
        visibility = ['//visibility:public'],
    )

