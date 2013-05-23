#!/usr/bin/env perl
#
# PVer
# A source revision fetcher and pre-processor.
#
# Copyright (C) 2013 Jean-Charles Lefebvre <jcl [AT] jcl [DOT] io>
#
# This software is provided 'as-is', without any express or implied
# warranty.  In no event will the authors be held liable for any damages
# arising from the use of this software.
#
# Permission is granted to anyone to use this software for any purpose,
# including commercial applications, and to alter it and redistribute it
# freely, subject to the following restrictions :
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
# Author:     Jean-Charles Lefebvre
# Created On: 2013-05-22 11:31:29Z
#
# $Id$
#

use strict;
use warnings;

use File::Basename ();
use Getopt::Long ();
use POSIX ();

use constant
{
    RESERVED_VARS => [ 'PROJECTDIR', 'REVISION', 'RCSNAME' ],
    TAG_PREFIX    => '@',
    TAG_SUFFIX    => '@',
};


#-------------------------------------------------------------------------------
sub usage
{
    my $msg        = shift;
    my $name       = File::Basename::basename($0);
    my $tag_prefix = TAG_PREFIX;
    my $tag_suffix = TAG_SUFFIX;

    $msg .= "\n\n" if defined $msg;
    $msg = '' unless defined $msg;

  die $msg, <<USAGE;
Usage:
    $name [options] {project_root_dir}

    Get the revision number/hash/string of a specified project root
    directory from the RCS (GIT or SVN) and allows to perform some
    printing and/or file formating operations with it.

    Although this script runs on Unix derived platforms, it is mainly meant to
    be used on Windows platform which *natively* lacks the convenience of those
    cat/date/head/... tools from the Unix world.

Requirements:
    A minimal install of Perl, only core modules are used.
    If a SVN repository is detected, the svnversion command must be in the PATH.
    If a GIT repository is detected, the git command must be in the PATH.

Options:
    --help, -h
        Print this message and leave.

    --define, -d, -D {expression}
        Define values to be used as a replacement.
        This option is only useful with the --template option.
        You can use this option several times.

        The {expression} can have the following formats:
        * {key=value}
          The simplest format, the string {value} is assigned to {key}.
        * {key=value|func[ arg]}
          The {value} of the variable {key} will be the preprocessed by {func},
          eventually according to its argument {arg} if needed.
          See the 'Functions' section for more info.
        * {key=func[ arg]}
          This is valid since some functions do not need an initial value
          (i.e.: the initial value is implicitely defined and empty).
        * {key=value|func1 arg1|func2}
          A more complex format, {value} is preprocessed by {func1}, the result
          of {func1} is preprocessed by {func2} which will be then the final
          value of the variable {key}.

        In case your literal value matches the name of a function, you can use
        the '%!' prefix (without quotes) to ensure the parser will not try to
        act as if it was your intent to call a function. Example:
        # $name -d "MYVAR=%!cat woman"
        As a result, MYVAR's value will be 'cat woman' instead of 'woman'.
        And you can still use in combination with other functions:
        # $name -d "MYVAR=%!cat woman|replace man men"
        MYVAR's value will be 'cat women'.

        Note that there are some predefined variables that you cannot redefine:
        * PROJECTDIR
          The project's root directory as it has been given.
        * REVISION
          The revision number/hash/string as it has been fetched from the RCS.
        * RCSNAME
          The name of the detected RCS (e.g.: 'git' or 'svn').

    --loosy
        Disable the default strict mode.
        If you do NOT specify this option, this script will try to ensure that
        your local copy of the repository is clean (i.e.: no modified files or
        structure) in order to continue. This can be useful to ensure everything
        is OK to generate a production release for example.
        This test is performed while fetching the revision number/hash/string
        from the RCS.

    --quiet, -q
        Do not print unnecessary info on STDERR.

    --tag-prefix {prefix}
        Replace the default tag prefix (default: $tag_prefix).
        Only useful with the --template option.

    --tag-suffix {suffix}
        Replace the default tag suffix (default: $tag_suffix).
        Only useful with the --template option.

    --template, -t {in_file}[;{out_file}]
        Find and replace every occurrences of every defined values.
        Input file can be '-' (minus) in which case input data will be read from
        STDIN. The same exception goes for output file, in which case the
        processed content of the input file will be written to STDOUT.
        If no output file is specified, the input file will be replaced.
        See the --define option to define more variables than the predefined
        ones.
        You can use this option several times.

Functions:
    The arguments of the following functions must be space-separated. Because of
    this, you may need to use the following specifiers (without quotes):
    * '%_': a whitespace
    * '%t': a tab
    * '%%': the '%' character

    cat {str}
        Append {str}.
    cut {delimiters} {fields}
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
    lc
        Convert to lower case.
    replace {to} {from}
        Replace every occurrences of {to} to {from}.
        Case sensitive.
    uc
        Convert to upper case.
    var {name}
        Append the value of the already defined variable {name}.
        When you use the --define option, variables are treated in the same
        order than in the command line.
    winver
        This function has two very different behaviors according to the use of
        the --loosy option:
        * strict mode: preprocess the current value in order to be compliant
          with Microsoft's VERSIONINFO specification (i.e.: 4 comma-separated
          16bit unsigned integers). The given value is expected to be a
          period-separated version string. If it contains less than 4 elements
          as required by Microsoft, '0' element(s) will be appended. If it
          contains more, it will be rejected and this script will leave (e.g.:
          "1.0.425" -> "1,0,425,0")
        * non-strict mode (--loosy): a more permissive version.
          * Out of boundaries numbers are capped to the [0,0xFFFF] range.
          * Input elements can be separated by a '.' or a ':' character.
          * Elements starting with non decimal characters are replaced by '0'.
          * Elements are interpreted as integer if they start with decimals.
            i.e.: the trailing part is removed from the element.
          * If more than 4 elements are specified, only the 4 first ones are
            used.
          * If less than 4 elements are specified, '0' elements are appended as
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
    my $file = shift;
    my $fh;

    if ($file eq '-')
    {
        $fh = *STDIN;
    }
    else
    {
        open($fh, "<$file")
            or die "Failed to open \"$file\"! $!\n";
        binmode($fh)
            or die "Failed to binmode($file)! $!\n";
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
            or die "Failed to open \"$file\"! $!\n";
        binmode($fh)
            or die "Failed to binmode($file)! $!\n";
    }

    print $fh $content;
    close $fh unless $file eq '-';
    $fh = undef;
}



#-------------------------------------------------------------------------------
sub preprocessor_cat
{
    my $ctx = shift;
    return 1 if exists $ctx->{query_argscount};
    return $ctx->{value}.$ctx->{args}[0];
}

#-------------------------------------------------------------------------------
sub preprocessor_cut
{
    my $ctx = shift;
    return 2 if exists $ctx->{query_argscount};

    my @delims        = split '', $ctx->{args}[0];
    my $delims_joined = join '', @delims;
    my $delims_regex  = qr/[\Q$delims_joined\E]/;
    my @fields        = split $delims_regex, $ctx->{value}, -1;
    my @ranges;
    my @selected;

    die "Invalid 'cut' delimiters list from expression \"$ctx->{exp}\"!\n"
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

        die "Invalid 'cut' fields list from expression \"$ctx->{exp}\"!\n"
            unless defined($min) and $min >= 1 and $min <= $max;

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
    die "Cannot append non defined variable \"$name\" from expression \"$ctx->{exp}\"!\n"
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

        die "Invalid version number \"$ver\" from expression \"$ctx->{exp}\"!\n"
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
    my $error = "Incorrect number of argument for function \"$func_name\" in expression \"$expression\"!\n";
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
    help     => undef,
    quiet    => undef,
    strict   => undef,
    define   => [ ],
    template => [ ],

    root_dir   => undef,
    tag_prefix => TAG_PREFIX,
    tag_suffix => TAG_SUFFIX,
    vars       => { },
);

BEGIN { $| = 1; }

# parse parameters
{
    my $res = Getopt::Long::GetOptions(
        'help|h'        => \$ctx{help},
        'define|d=s@'   => \$ctx{define},
        'loosy'         => \$ctx{strict},
        'quiet|q'       => \$ctx{quiet},
        'tag-prefix=s'  => \$ctx{tag_prefix},
        'tag-suffix=s'  => \$ctx{tag_suffix},
        'template|t=s@' => \$ctx{template},
    );

    usage unless $res and not $ctx{help};
    delete $ctx{help};

    usage unless @ARGV > 0;
    $ctx{root_dir} = shift @ARGV;
    usage "Root directory \"$ctx{root_dir}\" not found!"
        unless -d $ctx{root_dir};
    $ctx{vars}{PROJECTDIR} = $ctx{root_dir};

    $ctx{strict} = $ctx{strict} ? undef : 1;

    usage "Tag prefix cannot be empty!"
        unless length($ctx{tag_prefix}) > 0;
    usage "Tag suffix cannot be empty!"
        unless length($ctx{tag_suffix}) > 0;
}

# get revision number according to the type of RCS
if (-d "$ctx{root_dir}/.svn" or -d "$ctx{root_dir}/_svn")
{
    my $cmd    = "svnversion \"$ctx{root_dir}\"";
    my @result = qx/svnversion "$ctx{root_dir}"/;
    my $code   = $?;
    my $revision;

    die "Something bad happened while running command: $cmd (code ", ($code >> 8), ")\n"
        unless $code == 0;
    chomp $result[0];
    $revision = $result[0];
    if ($revision =~ /^(\d+)$/)
    {
        $revision = $1;
    }
    elsif ($ctx{strict})
    {
        die "Your local copy is not clean (i.e.: modified or switched), ",
            "please try to update first or perform a complete SVN checkout ",
            "(svnversion: $result[0])!\n";
    }

    $ctx{vars}{RCSNAME}  = 'svn';
    $ctx{vars}{REVISION} = $revision;
}
elsif (-d "$ctx{root_dir}/.git" or -d "$ctx{root_dir}/_git")
{
    # TODO: support git
    die "Git system not supported!";

    $ctx{vars}{RCSNAME}  = 'git';
    $ctx{vars}{REVISION} = 0;
}
die "RCS not supported or $ctx{root_dir} is not a local copy of a repository!"
    unless defined $ctx{vars}{REVISION};
warn uc($ctx{vars}{RCSNAME}), " detected.\n"
    unless $ctx{quiet};

# define more variables if needed
foreach my $expression (@{$ctx{define}})
{
    my ($key, $command) = split /=/, $expression, 2;

    $command = 1 #die "Invalid key=value or key=command pair \"$expression\"!\n"
        unless defined $command;
    die "Invalid variable name \"$key\"!\n"
        unless $key =~ /^\w+$/;
    die "Reserved variable \"$key\" cannot be redefined!\n"
        if grep { $key eq $_ } @{RESERVED_VARS()};

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
        my ($func, $func_name, $ref_args) = parse_expblock shift(@stack), $expression;

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
if (!$ctx{quiet} and keys %{$ctx{vars}} > 0)
{
    my $maxlen = 0;
    foreach (keys %{$ctx{vars}})
    {
        my $len = length;
        $maxlen = $len if $len > $maxlen;
    }
    warn "Variables:\n";
    warn sprintf('  %-'.$maxlen.'s', $_), " $ctx{vars}{$_}\n"
        foreach (sort keys(%{$ctx{vars}}));
}
#die dbg_dump(\%ctx), "\n";

# process templates if needed...
foreach my $expression (@{$ctx{template}})
{
    my ($src_file, $dst_file) = split /;/, $expression;
    my $content;

    # slurp file entirely to avoid altering it (new line sequence)
    $dst_file = $src_file unless defined $dst_file;
    $content  = file_slurp $src_file;
    warn "Processing $src_file\n        to $dst_file\n"
        unless $ctx{quiet};

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

# ... or just output revision string
print $ctx{vars}{REVISION}, "\n"
    unless @{$ctx{template}} > 0;

exit 0;
