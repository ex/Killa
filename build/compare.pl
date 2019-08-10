##==============================================================================
## Transform source code to compare with upstream
## Author: Laurens Rodriguez.
##------------------------------------------------------------------------------
use strict;
use warnings;
use diagnostics;

use File::Basename;
use File::Copy::Recursive qw( fcopy );
use File::Path qw( make_path rmtree );

my $SOURCE_PATH = '../src';
my $COMPARE_PATH = '../temp';

rmtree( $COMPARE_PATH );
make_path( "$COMPARE_PATH/killa" );
recurse( $SOURCE_PATH, \&processKillaFile );

##------------------------------------------------------------------------------
sub processKillaFile
{
    my $origFile = $_[0];
    my $destFile = basename( $_[0] );
    $destFile =~ s/killa/lua/;
    $destFile =~ s/^k(\S+)\./l$1\./;
    $destFile = "$COMPARE_PATH/killa/$destFile";
    print "$destFile\n";

    open( my $file, '<', $origFile ) or die( "Can't open $origFile: $!" );
    open( my $temp, '>', $destFile ) or die( "Can't create $destFile: $!" );
    while ( my $line = <$file> )
    {
        $line =~ s/killa/lua/g;
        $line =~ s/KILLA/LUA/g;
        print( $temp $line );
    }
    close( $file );
    close( $temp );
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

