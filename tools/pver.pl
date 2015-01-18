#!/usr/bin/env perl
#
# PVer
# A source revision fetcher and pre-processor.
#
# Copyright 2013-2015 Jean-Charles Lefebvre <polyvertex@gmail.com>
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions:
#
# 1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software
#    in a product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
# 2. Altered source versions must be plainly marked as such, and must not
#    be misrepresented as being the original software.
# 3. This notice may not be removed or altered from any source distribution.
#
#
# $Id: pver.pl 44 2015-01-18 21:42:25Z jcl $
#

use strict;
use warnings;
require 5.010;

use Cwd ();
use File::Basename ();
use Getopt::Long ();
use POSIX ();

use constant
{
    TAG_PREFIX => '@',
    TAG_SUFFIX => '@',
};


#-------------------------------------------------------------------------------
sub usage
{
    my $msg        = shift;
    my $name       = File::Basename::basename(__FILE__);
    my $tag_prefix = TAG_PREFIX;
    my $tag_suffix = TAG_SUFFIX;

    $msg .= "\n\n" if defined $msg;
    $msg = '' unless defined $msg;

  die $msg, <<USAGE;
Usage:
    $name [options] [infile1[;outfile1]] ...

    Sort of 'sed' command combined with some RCS detection and variables
    definition and manipulation.

    This script reads the current status of a specified project directory
    managed with GIT or SVN (basically hash/tag/revision and if working dir is
    dirty), and allows to perform some file transformation using these
    informations.

    A typical use is release versioning.

    Although this script is compatible with both Unix and Windows platforms, it
    has been developed with Windows in mind, which natively lacks the
    convenience of those sed/grep/... tools from the Unix world.

    Actually, if you're on Unix derived platform, avoid this script at any cost.

Requirements:
    A minimal install of Perl, only core modules are used.
    If a SVN repository is detected, the svnversion command must be in the PATH.
    If a GIT repository is detected, the git command must be in the PATH.

Input/Output:
    Unless you specify at least one input file to be processed, default behavior
    is to read STDIN, perform any required transformation, then write to STDOUT.

    The transformation step consists of replacing every occurrences of every
    defined value (see --define option) that is decorated with --tag-prefix and
    --tag-suffix.

    Input files can be '-' (hyphen) in which case input data will be read from
    STDIN. The same exception goes for output file, in which case the processed
    content of the input file will be written to STDOUT.
    If no output file is specified, output defaults to STDOUT.

Options:
    --help, -h
        Print this message and leave.

    -C {project_root_dir}
        Specify the working directory. Default is '.'.

    --define, -d, -D {expression}
        Define values to be used during transformation.
        You can use this option several times.

        The {expression} can have the following formats:
        * {key=value}
          The simplest format, the string {value} is assigned to a variable
          named 'key'.
        * {key=value|func[ args]}
          The {value} of the variable {key} will be processed by {func}.
          See the 'Functions' section for more info.
        * {key=func[ args]}
          This is valid since some functions do not need an initial value
          (i.e.: the initial value is implicitely defined and empty).
        * {key=value|func1 arg1|func2}
          A more complex format, {value} is processed by {func1}, the result of
          {func1} is then processed by {func2}. Its result will be the value
          assigned to the variable {key}.

        In case your literal value matches the name of a function, you can use
        the '%!' prefix (without quotes) to ensure the parser will not try to
        act as if it was your intent to call a function. Example:
        # $name -d "MYVAR=%!append period"
        As a result, MYVAR's value will be 'append period' instead of 'period'.
        And you can still use in combination with other functions:
        # $name -d "MYVAR=%!append period|replace period comma"
        MYVAR's value will be 'append comma'.

        Note that there are some predefined variables that you cannot redefine:
        * PROJECTDIR
          The project's root directory as it has been given.
        * RCSNAME
          The name of the detected RCS: 'git', 'svn' or 'unknown'.
        * REVISION
          The revision number/hash as fetched from the RCS.
          In case of a git hash, the full length hash is always assigned here.
          Use the GIT_SHORT variable to get the abbreviated form.
        * DIRTY
          Indicates a non clean working directory. Added/Modified files haven't
          been committed. Value can be 0 or 1. It will always be 0 in strict
          mode or if RCSNAME is 'unknown'.
        * GIT_SHORT
          The abbreviated form of the last commit hash.
          Its length is variable, usually 7, because git uses as many digits as
          needed to form a unique object name.
        * GIT_DESCRIBE
          The resulting string of the 'git describe' command.
        * GIT_BRANCH
          The name of the currently selected branch.
        * GIT_TAG
          The name of the last tag directly fetched from git.
        * GIT_TAGCOMMITS
          The number of commits on the top of the last found tag.
          This script fails if this value is not null in strict mode.
        * GIT_TAGVERSION
          If a version number could have been extracted from the name of the
          tag, it will be stored in this variable.
          This script fails if this value is null in strict mode.
        * GIT_TAGTITLE
          The title part of the tag name if there is any.
          Example: in tag 'v0.1-hello-world', the title would be 'hello-world'.
        * GIT_FIRST
          The full hash of the root commit in git.
          You must use the --git-lifespan option to have this variable defined.
        * GIT_FIRST_UNIX
          The unix timestamp of the root commit in git.
          This is the commit date, not author date.
          You must use the --git-lifespan option to have this variable defined.
        * GIT_LIFE_DAYS
          The number of days between the first commit to today.
          You must use the --git-lifespan option to have this variable defined.

    --force
        Force replace output files.
        Default behavior is to skip if file already exists.

    --list, -l
        Just print variables listing to STDOUT and leave.

    --strict
        Enable strict mode. In strict mode, this script will try to ensure that
        your local copy of the repository is clean (i.e.: no modified files or
        structure) before going further. This can be useful in case you want to
        build a production release for example.
        This test is performed while fetching the revision number/hash/string
        from the RCS.

    --quiet, -q
        Do not print info on stderr.

    --tag-prefix {prefix}
        Replace the default tag prefix (default: $tag_prefix).

    --tag-suffix {suffix}
        Replace the default tag suffix (default: $tag_suffix).

Git specific options:
    --git-alltags
        Instead of searching only for annotated tags, use any tag found in
        refs/tags namespace. This option enables matching a lightweight
        (non-annotated) tag.

    --git-lifespan
        Allows to get the GIT_FIRST, GIT_FIRST_UNIX and GIT_LIFE_DAYS variables.
        Crawling the complete history of a repository can take a while.

Functions:
    The arguments of the following functions must be space-separated. Because of
    this, you may need to use the following specifiers (without quotes):
    * '%_': a whitespace
    * '%t': a tab
    * '%%': the '%' character

    append {str}
        Append {str}.
    date {zone} [format]
        Append a formated date/time string.
        The {zone} argument can be 'unix', 'utc' or 'local'.
        If {zone} is 'unix', the [format] argument is ignored and a standard
        unix-epoch timestamp will be appended (always in UTC).
        Otherwise, the [format] must be specified and its content is directly
        passed to the POSIX::strftime() function unless it is equal to the
        following special values (without quotes):
        * 'db':  alias for '%Y-%m-%d %H:%M:%S'
        * 'iso': ISO 8601 format, alias for '%Y-%m-%dT%H:%M:%SZ', always in UTC
        Be aware that some strftime() format specifiers are not portable
        (e.g.: %F, %T, ...).
    env {name}
        Append the value of the environment variable {name}.
    head {file}
        Append the first line of {file}.
    increment [n]
        Increments the input value by one or [n].
        The input value will be rejected if it is not an (unsigned) integer.
    lc
        Convert to lower case.
    replace {to} {from}
        Replace every occurrences of {to} to {from}.
        Case sensitive.
    split {delimiters} {fields}
        Cut current value using the list of specified {delimiters} to get only
        the specified positional {fields}.
        {delimiters} must be one or several non-separated characters, the first
        specified delimiter will be used to join back the resulting string.
        Remember that you can use the '%_' sequence to specify a whitespace.
        {fields} takes the same format than Unix' 'cut' command, it must be one
        or more numbers or ranges (two numbers separated by a dash) separated by
        commas. Fields are numbered starting at 1. Incomplete ranges may be
        given: '-M' means '1-M', 'N-' means 'N' through end of last field. The
        list elements can be repeated, can overlap, and can be specified in any
        order; but the selected input is written in the same order that it is
        read, and is written exactly once.
    substr {ranges}
        Returns a substring of the given input value according to the {ranges}
        argument. The expected format for the {ranges} argument is the same as
        the {fields} argument for the 'split' function.
    uc
        Convert to upper case.
    var {name}
        Append the value of the already defined variable {name}.
        When you use the --define option, variables are treated in the same
        order than in the command line.
    winver
        This function behaves differently whether the --strict option is used:
        * strict mode: preprocess the current value in order to be compliant
          with Microsoft's VERSIONINFO specification (i.e.: 4 comma-separated
          16bit unsigned integers). The given value is expected to be a
          dot-separated version string. If it contains less than 4 elements
          as required by Microsoft, null elements will be appended as much as
          needed. If it contains more, it will be rejected and this script will
          leave (e.g.: "1.0.425" -> "1,0,425,0")
        * otherwise, the function is more permissive:
          * Out of boundaries numbers are not rejected but capped to the
            [0,0xFFFF] range.
          * Elements starting with non decimal characters are not rejected but
            replaced by '0'.
          * Input elements can be separated by a '.' or a ':' character. This is
            useful for SVN's dirty revision string (e.g.: 4123:4168MS).
          * Alphanumeric elements are interpreted as integer if they start with
            decimals instead of being rejected. The trailing part is removed.
          * If more than 4 elements are specified, only the 4 first ones are
            used.
          * If less than 4 elements are specified, null elements are appended as
            much as necessary.

USAGE
}

#-------------------------------------------------------------------------------
sub dbg_dump
{
    require Data::Dumper;
    return Data::Dumper::Dumper(@_);
}

#-------------------------------------------------------------------------------
sub file_first_line
{
    my $file = shift;
    open my $fh, "<$file" or die "Failed to open file \"$file\"! $!\n";
    my $line = <$fh>;
    chomp $line;
    return $line;
}

#-------------------------------------------------------------------------------
sub file_slurp
{
    my ($file, $binmode) = shift;
    my $fh;

    if ($file eq '-')
    {
        $fh = *STDIN;
    }
    else
    {
        open($fh, "<$file")
            or die "Failed to open \"$file\"! $!\n";
        if ($binmode)
        {
            binmode($fh)
                or die "Failed to binmode($file)! $!\n";
        }
    }

    local $/;
    <$fh>;
}

#-------------------------------------------------------------------------------
sub file_burp
{
    my ($file, $content) = @_;
    my $fh;

    if ($file eq '-')
    {
        $fh = *STDOUT;
    }
    else
    {
        open($fh, ">$file")
            or die "Failed to create \"$file\"! $!\n";
        binmode($fh)
            or die "Failed to binmode($file)! $!\n";
    }

    print $fh $content;
    close $fh unless $file eq '-';
    $fh = undef;
}



#-------------------------------------------------------------------------------
sub preprocessor_append
{
    my $ctx = shift;
    return 1 if exists $ctx->{query_argscount};
    return $ctx->{value}.$ctx->{args}[0];
}

#-------------------------------------------------------------------------------
sub preprocessor_date
{
    my $ctx = shift;
    return [ 1, 2 ] if exists $ctx->{query_argscount};

    my ($zone, $fmt) = $ctx->{args}[0..1];

    die "Unknown zone \"$zone\" in expression \"$ctx->{exp}\"!\n"
        unless grep { $zone eq $_ } (qw(unix utc local));

    return $ctx->{value}.time() if $zone eq 'unix';

    die "Invalid parameters for function \"$ctx->{name}\" in expression \"$ctx->{exp}\"!\n"
        unless @{$ctx->{args}} == 2;

    if ($fmt eq 'db')
    {
        $fmt = '%Y-%m-%d %H:%M:%S';
    }
    elsif ($fmt eq 'iso')
    {
        $zone = 'utc';
        $fmt  = '%Y-%m-%dT%H:%M:%SZ';
    }

    my @atime = ($zone eq 'utc') ? gmtime(time) : localtime(time);

    return $ctx->{value}.POSIX::strftime($fmt, @atime);
}

#-------------------------------------------------------------------------------
sub preprocessor_env
{
    my $ctx = shift;
    return 1 if exists $ctx->{query_argscount};

    my $name = $ctx->{args}[0];
    warn "Environment variable \"$name\" not defined!\n"
        unless defined $ENV{$name};
    return $ctx->{value}.$ENV{$name};
}

#-------------------------------------------------------------------------------
sub preprocessor_head
{
    my $ctx = shift;
    return 1 if exists $ctx->{query_argscount};
    return $ctx->{value}.file_first_line($ctx->{args}[0]);
}

#-------------------------------------------------------------------------------
sub preprocessor_increment
{
    my $ctx = shift;
    return [ 0, 1 ] if exists $ctx->{query_argscount};

    my $increment = $ctx->{args}[0];
    $increment = 1 unless defined $increment;

    die "Integer expected in expression \"$ctx->{exp}\"!\n"
        unless $ctx->{value} =~ /^\d+$/;

    return $ctx->{value} + $increment;
}

#-------------------------------------------------------------------------------
sub preprocessor_lc
{
    my $ctx = shift;
    return 0 if exists $ctx->{query_argscount};
    return lc $ctx->{value};
}

#-------------------------------------------------------------------------------
sub preprocessor_replace
{
    my $ctx = shift;
    return 2 if exists $ctx->{query_argscount};

    my ($value, $from, $to) = ( $ctx->{value}, @{$ctx->{args}}[0..1] );
    while ((my $pos = index($value, $from)) >= 0)
    {
        substr $value, $pos, length($from), $to;
    }
    return $value;
}

#-------------------------------------------------------------------------------
sub preprocessor_split
{
    my $ctx = shift;
    return 2 if exists $ctx->{query_argscount};

    my @delims        = split '', $ctx->{args}[0];
    my $delims_joined = join '', @delims;
    my $delims_regex  = qr/[\Q$delims_joined\E]/;
    my @fields        = split $delims_regex, $ctx->{value}, -1;
    my @ranges;
    my @selected;

    die "Invalid delimiters list in expression \"$ctx->{exp}\"!\n"
        unless @delims > 0;

    foreach my $range (split /\,+/, $ctx->{args}[1])
    {
        my $min;
        my $max;

        if ($range =~ /^(\d+)?\-(\d+)?$/)
        {
            ($min, $max) = ($1, $2);
            $min = 1 unless defined $min;
            $max = scalar(@fields) unless defined $max;
        }
        elsif ($range =~ /^(\d+)$/)
        {
            $min = $1;
            $max = $min;
        }

        die "Invalid fields list in expression \"$ctx->{exp}\"!\n"
            unless defined($min) and defined($max)
            and $min >= 1 and $min <= $max;

        $max = scalar(@fields) if $max > @fields;
        --$min; --$max;

        push @ranges, [ $min, $max ];
    }

    for (my $i = 0; $i < @fields; ++$i)
    {
        foreach my $ref_range (@ranges)
        {
            if ($i >= $ref_range->[0] and $i <= $ref_range->[1])
            {
                push @selected, $fields[$i];
                last;
            }
        }
    }

    return join $delims[0], @selected;
}

#-------------------------------------------------------------------------------
sub preprocessor_substr
{
    my $ctx = shift;
    return 1 if exists $ctx->{query_argscount};

    my @ranges;
    my $result = '';

    foreach my $range (split /\,+/, $ctx->{args}[0])
    {
        my $min;
        my $max;

        if ($range =~ /^(\d+)?\-(\d+)?$/)
        {
            ($min, $max) = ($1, $2);
            $min = 1 unless defined $min;
            $max = length($ctx->{value}) unless defined $max;
        }
        elsif ($range =~ /^(\d+)$/)
        {
            $min = $1;
            $max = $min;
        }

        die "Invalid range in expression \"$ctx->{exp}\"!\n"
            unless defined($min) and defined($max)
            and $min >= 1 and $min <= $max;

        $max = length($ctx->{value}) if $max > length($ctx->{value});
        --$min; --$max;

        push @ranges, [ $min, $max ];
    }

    foreach my $ref_range (@ranges)
    {
        my $len = $ref_range->[1] - $ref_range->[0] + 1; # $max is inclusive
        $result .= substr $ctx->{value}, $ref_range->[0], $len;
    }

    return $result;
}

#-------------------------------------------------------------------------------
sub preprocessor_uc
{
    my $ctx = shift;
    return 0 if exists $ctx->{query_argscount};
    return uc $ctx->{value};
}

#-------------------------------------------------------------------------------
sub preprocessor_var
{
    my $ctx = shift;
    return 1 if exists $ctx->{query_argscount};

    my $name = $ctx->{args}[0];
    die "Cannot append non defined variable \"$name\" in expression \"$ctx->{exp}\"!\n"
        unless exists $ctx->{global}{vars}{$name};
    return $ctx->{value}.$ctx->{global}{vars}{$name};
}

#-------------------------------------------------------------------------------
sub preprocessor_winver
{
    my $ctx = shift;
    return 0 if exists $ctx->{query_argscount};

    if ($ctx->{global}{strict})
    {
        my $ver = $ctx->{value};
        my @a   = split /\./, $ver;

        die "Invalid version number \"$ver\" in expression \"$ctx->{exp}\"!\n"
            unless $ver =~ /^\d+(\.\d+){0,3}$/
            and @a <= 4
            and grep { /^\d+$/ and $_ >= 0 and $_ <= 0xFFFF } @a == scalar @a;
        while (@a < 4) { push @a, 0; }

        return join ',', @a;
    }
    else
    {
        my $ver = $ctx->{value};
        my @a   = split /[\.\:]/, $ver, 4;

        foreach my $e (@a)
        {
            $e = $1     if $e =~ /^(\d+)[^\d]*$/;
            $e = 0      unless $e =~ /^\d+$/;
            $e = 0      if $e < 0;
            $e = 0xFFFF if $e > 0xFFFF;
        }
        while (@a < 4) { push @a, 0; }

        return join ',', @a;
    }
}



#-------------------------------------------------------------------------------
sub is_preprocessor
{
    my ($name) = split /\s+/, shift;
    return
        (defined($name) and $name =~ /^\w+$/) ?
        main->can('preprocessor_'.$name) :
        undef;
}

#-------------------------------------------------------------------------------
sub parse_expblock
{
    my ($block, $expression) = @_;
    my ($func_name, @args) = split /\s+/, $block;
    my $func = is_preprocessor $func_name;

    return ( undef, $block ) unless $func;

    my $argscount = $func->({ query_argscount => 1 });
    my $error = "Incorrect number of arguments for function \"$func_name\" in expression \"$expression\"!\n";
    if (ref($argscount) eq 'ARRAY')
    {
        my ($min, $max) = @$argscount;
        die unless $min <= $max and $max >= $min;
        die $error unless @args >= $min and @args <= $max;
    }
    else
    {
        die $error unless scalar(@args) == $argscount;
    }

    foreach my $a (@args)
    {
        $a =~ s/%_/ /g;
        $a =~ s/%t/\t/g;
        $a =~ s/%%/%/g;
    }

    return ( $func, $func_name, \@args );
}



#-------------------------------------------------------------------------------
my %ctx = ( # global context
    help   => undef,
    force  => undef,
    quiet  => undef,
    strict => undef,
    list   => undef,
    define => [ ],

    call_dir      => undef,
    root_dir      => undef,
    git_alltags   => undef,
    git_lifespan  => undef,
    tag_prefix    => TAG_PREFIX,
    tag_suffix    => TAG_SUFFIX,
    vars          => { },
    reserved_vars => { },
);

BEGIN { $| = 1; }
END { Cwd::chdir($ctx{call_dir}) if defined $ctx{call_dir}; }

# parse parameters
{
    my $res = Getopt::Long::GetOptions(
        'help|h'       => \$ctx{help},
        'C=s'          => \$ctx{root_dir},
        'define|d=s@'  => \$ctx{define},
        'force'        => \$ctx{force},
        'git-alltags'  => \$ctx{git_alltags},
        'git-lifespan' => \$ctx{git_lifespan},
        'list|l'       => \$ctx{list},
        'quiet|q'      => \$ctx{quiet},
        'strict'       => \$ctx{strict},
        'tag-prefix=s' => \$ctx{tag_prefix},
        'tag-suffix=s' => \$ctx{tag_suffix},
    );

    usage unless $res and not $ctx{help};
    delete $ctx{help};

    if (defined $ctx{root_dir})
    {
        usage "Project directory \"$ctx{root_dir}\" not found!"
            unless -d $ctx{root_dir};
        $ctx{vars}{PROJECTDIR} = $ctx{root_dir};
    }
    else
    {
        $ctx{root_dir} = '.';
        $ctx{vars}{PROJECTDIR} = '.';
    }

    usage "Tag prefix cannot be empty!"
        unless length($ctx{tag_prefix}) > 0;
    usage "Tag suffix cannot be empty!"
        unless length($ctx{tag_suffix}) > 0;
}

# chdir
unless ($ctx{root_dir} eq '.')
{
    $ctx{call_dir} = Cwd::cwd;
    Cwd::chdir($ctx{root_dir})
        or die "Failed to chdir to \"$ctx{root_dir}\"! $!\n";
}

# get revision number according to the type of RCS
if (-d '.svn' or -d '_svn')
{
    my $cmd    = 'svnversion';
    my @result = qx/$cmd/;
    my $code   = $?;
    my $dirty  = 0;
    my $revision;

    die "Something went wrong running command: $cmd (code ", ($code >> 8), ")\n"
        unless $code == 0;
    chomp $result[0];
    $revision = $result[0];
    if ($revision =~ /^(\d+)$/)
    {
        $revision = $1;
    }
    else
    {
        $dirty = 1;
        if ($ctx{strict})
        {
            die "Your local copy is not clean (i.e.: modified or switched), ",
                "please try to update first or perform a complete SVN checkout ",
                "(svnversion: $result[0])!\n";
        }
    }

    $ctx{vars}{RCSNAME}  = 'svn';
    $ctx{vars}{REVISION} = $revision;
    $ctx{vars}{DIRTY}    = $dirty;
}
elsif (-d '.git' or -d '_git')
{
    # examples of output from 'describe':
    # * 2.2.0
    # * v2.2.0
    # * v2.2.0-dirty
    # * v2.2.0-1-g4ca113d6a2844260d6ba7737697d404e9256cd90
    # * v2.2.0-1-g4ca113d6a2844260d6ba7737697d404e9256cd90-dirty
    # * 4ca113d6a2844260d6ba7737697d404e9256cd90
    # * 4ca113d6a2844260d6ba7737697d404e9256cd90-dirty
    # * arbitrary-tag-name
    # * arbitrary-tag-name-dirty

    $ctx{vars}{RCSNAME}        = 'git';
    $ctx{vars}{REVISION}       = '';
    $ctx{vars}{DIRTY}          = 0;
    $ctx{vars}{GIT_DESCRIBE}   = '';
    $ctx{vars}{GIT_SHORT}      = '';
    $ctx{vars}{GIT_BRANCH}     = '';
    $ctx{vars}{GIT_TAG}        = '';
    $ctx{vars}{GIT_TAGCOMMITS} = 0;
    $ctx{vars}{GIT_TAGVERSION} = '';
    $ctx{vars}{GIT_TAGTITLE}   = '';
    #$ctx{vars}{GIT_FIRST}      = 'Use the --git-lifespan option to get this value';
    #$ctx{vars}{GIT_FIRST_UNIX} = 0;
    #$ctx{vars}{GIT_LIFE_DAYS}  = 0;

    # describe head
    {
        my $cmd =
            'git describe '.
            ($ctx{git_alltags} ? '--tags ' : '').
            '--always --abbrev --dirty=-dirty';
        my @result = qx/$cmd/;
        my $code = $?;

        die "Something went wrong while running command: $cmd (code ", ($code >> 8), ")\n"
            unless $code == 0;

        chomp $result[0];
        $ctx{vars}{GIT_DESCRIBE} = $result[0];
    }

    # head commit
    {
        my $cmd = 'git rev-parse HEAD';
        my @result = qx/$cmd/;
        my $code = $?;

        die "Something went wrong while running command: $cmd (code ", ($code >> 8), ")\n"
            unless $code == 0;
        die "Could not interpret result of call: $cmd\n"
            unless $result[0] =~ /^\s*([a-h\d]{4,40})\s*$/;

        $ctx{vars}{REVISION} = $1;
    }

    # head branch name
    {
        my $cmd = 'git branch --list --no-color --contains HEAD';
        my @result = qx/$cmd/;
        my $code = $?;

        die "Something went wrong while running command: $cmd (code ", ($code >> 8), ")\n"
            unless $code == 0;

        # there might be several branches, but the one we want is the currently
        # selected branch, which is flagged with a '*' prefix
        foreach (@result)
        {
            if (/^\*\s+([^\s]+)\s*$/)
            {
                $ctx{vars}{GIT_BRANCH} = $1;
                last;
            }
        }
        die "Could not interpret result of call: $cmd\n"
            unless defined($ctx{vars}{GIT_BRANCH})
            and length($ctx{vars}{GIT_BRANCH}) > 0;
    }

    # get the first commit ever
    # a.k.a "get the first root commit we can find and hope it's the only one or
    # the right one"
    if ($ctx{git_lifespan})
    {
        my $cmd = 'git rev-list --max-parents=0 HEAD';
        my @result = qx/$cmd/;
        my $code = $?;

        die "Something went wrong while running command: $cmd (code ", ($code >> 8), ")\n"
            unless $code == 0;
        die "Could not interpret result of call: $cmd\n"
            unless $result[0] =~ /^\s*([a-h\d]{40})\s*$/;

        $ctx{vars}{GIT_FIRST} = $1;

        $cmd = 'git show -s --format="%ct" '.$ctx{vars}{GIT_FIRST};
        @result = qx/$cmd/;
        $code = $?;

        die "Something went wrong while running command: $cmd (code ", ($code >> 8), ")\n"
            unless $code == 0;
        die "Could not interpret result of call: $cmd\n"
            unless $result[0] =~ /^\s*(\d+)\s*$/;

        $ctx{vars}{GIT_FIRST_UNIX} = $1;
        $ctx{vars}{GIT_LIFE_DAYS}  =
            (time() >= $ctx{vars}{GIT_FIRST_UNIX}) ?
            int((time() - $ctx{vars}{GIT_FIRST_UNIX}) / 86400) : 0;
    }

    if ($ctx{vars}{GIT_DESCRIBE} =~ /^([a-h\d]{4,40})(\-dirty)?$/)
    {
        # here, no tag was found
        # possible forms:
        # * 4ca113d
        # * 4ca113d-dirty
        $ctx{vars}{GIT_SHORT} = $1;
        $ctx{vars}{DIRTY}      = defined($2) ? 1 : 0;
    }
    elsif ($ctx{vars}{GIT_DESCRIBE} =~ /^(.+)\-(\d+)\-g?([a-h\d]{4,40})(\-dirty)?$/)
    {
        # here, a tag was found and there have been commits on the top of it
        # possible forms:
        # * v2.2.0-1-g4ca113d
        # * v2.2.0-1-g4ca113d-dirty
        $ctx{vars}{GIT_TAG}        = $1;
        $ctx{vars}{GIT_TAGCOMMITS} = $2;
        $ctx{vars}{GIT_SHORT}      = $3;
        $ctx{vars}{DIRTY}          = defined($4) ? 1 : 0;
    }
    elsif ($ctx{vars}{GIT_DESCRIBE} =~ /^(.+?)(\-dirty)?$/)
    {
        # +? -> match 1 or more times, NOT GREEDILY
        # this is important to handle BOTH cases (i.e.: with or without the
        # "-dirty" suffix)

        # here, HEAD is on a tag, only its name has been printed
        $ctx{vars}{GIT_TAG} = $1;
        $ctx{vars}{DIRTY}   = defined($4) ? 1 : 0;
    }

    die "Strict mode does not allow non-tagged revisions. Git's output was: ".$ctx{vars}{GIT_DESCRIBE}."\n"
        if $ctx{strict} and length($ctx{vars}{GIT_TAG}) == 0;
    die "Strict mode requires the last commit to be tagged. Disable strict mode to bypass.\n"
        if $ctx{strict} and $ctx{vars}{GIT_TAGCOMMITS} > 0;

    # call rev-parse again to let git abbrev the hash
    if (length($ctx{vars}{GIT_SHORT}) == 0)
    {
        my $cmd = 'git rev-parse --short HEAD';
        my @result = qx/$cmd/;
        my $code = $?;

        die "Something went wrong while running command: $cmd (code ", ($code >> 8), ")\n"
            unless $code == 0;

        chomp $result[0];
        $ctx{vars}{GIT_SHORT} = $result[0];

        die "Could not interpret result of call: $cmd\n"
            unless $ctx{vars}{GIT_SHORT} =~ /^([a-h\d]{4,40})$/;

        # paranoid check
        die unless
            substr($ctx{vars}{REVISION}, 0, length($ctx{vars}{GIT_SHORT}))
                eq $ctx{vars}{GIT_SHORT};
    }

    if ($ctx{vars}{GIT_TAG} =~ /^v?([\d+\.]*\d+)(?:[\-_]?(.+))?$/)
    {
        $ctx{vars}{GIT_TAGVERSION} = $1;
        $ctx{vars}{GIT_TAGTITLE} = $2 if defined($2);
    }

    die "Could not extract a version number from tag \"", $ctx{vars}{GIT_TAG}, "\"! Disable strict mode to bypass.\n"
        if $ctx{strict} and length($ctx{vars}{GIT_TAGVERSION}) == 0;
}
else
{
    my $error = "No supported RCS detected or $ctx{root_dir} is not a repository!\n";

    die $error if $ctx{strict};
    warn $error unless $ctx{list};

    $ctx{vars}{RCSNAME}  = 'unknown';
    $ctx{vars}{REVISION} = 0;
    $ctx{vars}{DIRTY}    = 0;
}
warn uc($ctx{vars}{RCSNAME}), " detected.\n"
    unless $ctx{quiet} or $ctx{list} or $ctx{vars}{RCSNAME} eq 'unknown';

# build the 'reserved variables' dictionary
$ctx{reserved_vars}{$_} = 1 foreach (keys %{$ctx{vars}});

# define more variables if needed
foreach my $expression (@{$ctx{define}})
{
    my ($key, $command) = split /=/, $expression, 2;

    # default value for defines
    $command = 1 unless defined $command;

    die "Invalid variable name \"$key\"!\n"
        unless $key =~ /^\w+$/;
    die "Reserved variable \"$key\" cannot be redefined!\n"
        if exists $ctx{reserved_vars}{$key};

    my @stack = split /\|/, $command;
    if (substr($stack[0], 0, 2) eq '%!')
    {
        $stack[0] = substr $stack[0], 2;
    }
    elsif (is_preprocessor $stack[0])
    {
        unshift @stack, '';
    }

    my $value = shift @stack;

    while (@stack > 0)
    {
        my ($func, $func_name, $ref_args) =
            parse_expblock shift(@stack), $expression;

        die "Unknown function in expression \"$expression\"!\n"
            unless defined $func;

        $value = $func->({
            name   => $func_name,
            args   => $ref_args,
            key    => $key,
            value  => $value,
            exp    => $expression,
            global => \%ctx,
        });
    }

    $ctx{vars}{$key} = $value;
}
delete $ctx{define};

# print variables listing if needed
if (($ctx{list} or not $ctx{quiet}) and keys %{$ctx{vars}} > 0)
{
    my $maxlen = 0;
    foreach (keys %{$ctx{vars}})
    {
        my $len = length;
        $maxlen = $len if $len > $maxlen;
    }

    my $fh = $ctx{list} ? *STDOUT : *STDERR;
    print { $fh } sprintf('%-'.$maxlen.'s', $_), " $ctx{vars}{$_}\n"
        foreach (sort keys(%{$ctx{vars}}));
}
#die dbg_dump(\%ctx), "\n";
exit 0 if $ctx{list};

# process templates if needed
@ARGV = ('-') unless @ARGV > 0;
foreach my $expression (@ARGV)
{
    my ($src_file, $dst_file) = split /\s*;\s*/, $expression;
    my $src_file_label;
    my $content;

    $src_file =~ s%^\s+%%;
    $src_file = '-'
        unless length($src_file) > 0;
    $dst_file = '-'
        unless defined($dst_file) and length($src_file) > 0;

    $src_file_label = $src_file eq '-' ? 'stdin' : $src_file;

    if ($dst_file ne '-' and not $ctx{force} and -e $dst_file)
    {
        warn "Skipping $src_file_label\n" unless $ctx{quiet};
        continue;
    }
    warn "Processing $src_file_label\n" unless $ctx{quiet};

    # slurp file entirely to avoid altering it (new line sequence)
    $content = file_slurp $src_file, ($dst_file eq '-' ? 0 : 1);

    # replace tags in a very unoptimized way
    while (my ($key, $value) = each %{$ctx{vars}})
    {
        my $tag = $ctx{tag_prefix}.$key.$ctx{tag_suffix};
        while ((my $pos = index($content, $tag)) >= 0)
        {
            substr $content, $pos, length($tag), $value;
        }
    }

    file_burp $dst_file, $content;
}

exit 0;
