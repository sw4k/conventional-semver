
**conventional-semver** is a [Conventional Commits](https://www.conventionalcommits.org/) processor written in C++, designed to emit [Semantic Versioning]() details as part of a build pipeline. It runs independent from larger frameworks/toolchains, allowing it to be used without requiring additional dependencies to be layered onto a build environment.

## Usage

### Generating SEMVER

When you run **conventional-semver** from within a `git` repository, it will automatically process the log and emit a semver.

Example:

```bash
% conventional-semver
0.1.23
```

This allows you to pull a semver into an Environment Variable, evaluate it as an argument, or pipe it to a file/stream for additional processing:

```bash
% export SEMVER=$(conventional-semver)
% echo $SEMVER
0.1.23
```

#### SEMVER Configuration

To keep SEMVER Configuration simple there are three conifguration files:

- **major-types.conf** - contains a listing of all Conventional Commit `<type>` strings which should result in a MAJOR increment.
- **minor-types.conf** - contains a listing of all Conventional Commit `<type>` strings which should result in a MINOR increment.
- **patch-types.conf** - contains a listing of all Conventional Commit `<type>` strings which should result in a PATCH increment.

Unrecognized `<type>` values result in no change to SEMVER result.

Each of these contains a default population which you can customize to meet your own needs needs.

### Generating CHANGELOG

To generate a CHANGELOG you specify the `--changelog [filename]` switch, this takes an optional filename argument. If no filename is provided a default filename of `./CHANGELOG` is used to emit a file into the current working directory.

Example:

```bash
% conventional-semver --changelog
```

#### CHANGELOG Templates

The CHANGELOG output is controlled through one or more templates. This includes a required "entry" template, and optional "header" and "footer" templates. Combined this is meant to provide enough flexibility that you could emit templates in various structured formats such as XML, JSON, Markdown, etc.

The default templates are meant to produce a generic TEXT file that is markdown-friendly.

##### CHANGELOG Entry Template

For each SEMVER increment a CHANGELOG Entry is emitted.

The format of each CHANGELOG Entry is taken from a file named `changelog-entry.template`, when `--changelog` is specified this file is required to be present or the command will fail.

This is the default content of this template:

```text
$(DATE) $(SEMVER)-$(HASH)
$(TYPE)$(SCOPE): $(MESSAGE)
```

The entry template supports the following expando variables:

| Expando | Comment |
|-|-|
| $(DATE) | The date of the commit which caused SEMVER increment. This is emitted in the default culture of the current environment. |
| $(SEMVER) | The calculated SEMVER value. |
| $(HASH) | The short-form git commit hash. |
| $(TYPE) | The Conventional Commits `<type>` value. |
| $(SCOPE) | If any, the Conventional Commits `[scope]` value, including parenthesis. |
| $(MESSAGE) | The commit message, sans `<type>` and `[scope]`. |
| $(BODY) | If any, the commit body. |
| $(TRAILERS) | If any, the commit trailers (footers). |

##### CHANGELOG Header Template

The header prepended to the CHANGELOG comes from a file named `changelog-header.template`.

There are no special expando variables supported in the header.

##### CHANGELOG Footer Template

The footer appended to the CHANGELOG comes from a file named `changelog-footer.template`.

There are no special expando variables supported in the footer.

##### CHANGELOG Template Caveats

No escaping is performed on any of the emitted values. Thus, it is possible for commit messages to interact with parsers/renderers in unintended ways. For example if you make a template to emit an HTML changelog, and a commit message contains content which looks like an 'element', it will most likely result in a changelog that doesn't render as expected.

### Configuration File Handling

**conventional-semver** will hierarchically scan for Configuration Files in the following order, if a file is not found scanning will progress in-order through these locaitons, if a file is found scanning will stop.

1. The current working directory that **conventional-semver** is run from.
2. A `.conventional-semver` in the current working directory.
3. The parent directory of the current working directory.
4. A `.conventional-semver` in the parent directory.
5. A traversal through all parent directories according to steps (3) and (4), up to and including the volume root.

To disable directory traversal you can specify the `--no-traversal` switch.

You can also specify the `--config-dir <path>` switch, providing the required `<path>` argument. The value provided for `<path>` will be checked for configuration files. When this switch is specified no directory traversal will be performed.

If a required configuration file cannot be found an error will be emitted and the tool will terminate with a non-zero exit code.

