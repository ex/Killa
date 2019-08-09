##==============================================================================
## Transform source code to compare with upstream
## Author: Laurens Rodriguez.
##------------------------------------------------------------------------------
use strict;
use warnings;
use diagnostics;

use File::Basename;
use File::Copy::Recursive qw( fcopy );
use File::Path qw( make_path );

my $SOURCE_PATH = '../src';
my $COMPARE_PATH = '../temp';

recurse( $SOURCE_PATH, \&processKillaFile );

##------------------------------------------------------------------------------
sub processKillaFile
{
    my $file = $_[0];
    my $copyFile = basename( $_[0] );
    $copyFile =~ s/killa/lua/;
    $copyFile =~ s/^k(\S+)\./l$1\./;
    make_path( "$COMPARE_PATH/killa" );
    print "$copyFile\n";
    fcopy( $file, "$COMPARE_PATH/killa/$copyFile" );
}

##------------------------------------------------------------------------------
sub recurse
{
    my $path = shift;
    my $onFileCallback = shift;
    my $scaped = $path =~ /".+"/;

    die( "Path too short: $path" ) if ( length( $path ) < 4 );
    if ( ( -e $path ) && ( ! -d $path ) )
    {
        $onFileCallback->( $path ) if ( defined $onFileCallback );
        return;
    }

    ## Append a trailing / if it's not there.
    $path .= '/' if ( $path !~ /\/$/ );

    ## Loop through the files contained in the directory.
    for my $eachFile ( glob( $path . '*' ) )
    {
        if ( -d $eachFile )
        {
            $eachFile = "\"$eachFile\"" if ( $scaped );

            ## If the file is a directory, continue recursive scan.
            recurse( $eachFile, $onFileCallback );
        }
        else
        {
            $onFileCallback->( $eachFile ) if ( defined $onFileCallback );
        }
    }
}

