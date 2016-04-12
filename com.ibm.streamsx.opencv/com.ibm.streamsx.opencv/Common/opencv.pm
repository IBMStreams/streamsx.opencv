# Copyright (C) 2012, 2016, International Business Machines Corporation
# All Rights Reserved

package opencv;

use strict ;
use util ;

require Exporter;
our @ISA = qw(Exporter);
our @EXPORT = qw(opencv_tb_declare opencv_tb_implement opencv_tb_init opencv_tb_get ipl_schema copy2tuple copy2img find_ipl_prefix) ;

my $mutex_created = 0 ;
my $ipl_prefix = "ipl_" ;

sub opencv_tb_declare($$)
{
  my ($model, $name) = @_ ;

  for(my $i=1; $i<$model->getNumberOfInputPorts( ); $i++)
  {
    my $ip = $model->getInputPortAt($i) ;

    if ($ip->getNumberOfAttributes( ) == 1)
    {
      my $atname = $ip->getAttributeAt(0)->getName( ) ;

      if ($atname eq "trackbar_${name}")
      {
        fail("'$name' is defined both as a parameter and an attribute of an input stream") if defined $model->getParameterByName($name) ;

        my $res = "" ;
        
        if (!$mutex_created)
        {
          $mutex_created = 1 ;
          
          $res = "SPL::Mutex _trackbar_mutex ;\n" ;
        }
        
        $res = $res
            . "int _auto_$atname ;\n" ;
        
        return $res ;
      }
    }
  }
  
  fail("'$name' is not a parameter nor an attribute of any input stream") unless defined $model->getParameterByName($name) ;

  my $val = get_param($model, $name, undef) ;

  fail("'$name' is undefined") unless defined $val ;
  
  return "" ;
}

sub opencv_tb_implement($$)
{
  my ($model, $name) = @_ ;

  my $opername = $model->getContext()->getKind();

  for(my $i=1; $i<$model->getNumberOfInputPorts( ); $i++)
  {
    my $ip = $model->getInputPortAt($i) ;

    if ($ip->getNumberOfAttributes( ) == 1)
    {
      my $atname = $ip->getAttributeAt(0)->getName( ) ;

      if ($atname eq "trackbar_${name}")
      {
        fail("'$name' is defined both as a parameter and an attribute of an input stream") if defined $model->getParameterByName($name) ;

        return
            "if(port == ${i})\n"
            . "{\n"
            . "  SPLLOG(L_TRACE, \"Entering \" \"${opername}::process IPort${i} ${atname}\", DBG_ASP);\n"
            . "  IPort${i}Type & p${i}tuple = static_cast<IPort${i}Type &>(tuple);\n"
            . "  AutoMutex lck(_trackbar_mutex) ;\n"
            . "  _auto_$atname = p${i}tuple.get_$atname( ) ;\n"
            . "  SPLLOG(L_TRACE, \"Exiting \" \"${opername}::process IPort${i} ${atname}\", DBG_ASP);\n"
            . "  return ;\n"
            . "}\n" ;
      }
    }
  }
  
  fail("'$name' is not a parameter nor an attribute of any input stream") unless defined $model->getParameterByName($name) ;

  my $val = get_param($model, $name, undef) ;

  fail("'$name' is undefined") unless defined $val ;
  
  return "" ;
}

sub opencv_tb_init($$)
{
  my ($model, $name) = @_ ;

  for(my $i=1; $i<$model->getNumberOfInputPorts( ); $i++)
  {
    my $ip = $model->getInputPortAt($i) ;

    if ($ip->getNumberOfAttributes( ) == 1)
    {
      my $atname = $ip->getAttributeAt(0)->getName( ) ;

      if ($atname eq "trackbar_${name}")
      {
        fail("'$name' is defined both as a parameter and an attribute of an input stream") if defined $model->getParameterByName($name) ;

        return ",_auto_$atname(0)\n" ;
      }
    }
  }
  
  fail("'$name' is not a parameter nor an attribute of any input stream") unless defined $model->getParameterByName($name) ;

  my $val = get_param($model, $name, undef) ;

  fail("'$name' is undefined") unless defined $val ;
  
  return "" ;
}

sub opencv_tb_get($$)
{
  my ($model, $name) = @_ ;

  for(my $i=1; $i<$model->getNumberOfInputPorts( ); $i++)
  {
    my $ip = $model->getInputPortAt($i) ;

    if ($ip->getNumberOfAttributes( ) == 1)
    {
      my $atname = $ip->getAttributeAt(0)->getName( ) ;

      if ($atname eq "trackbar_${name}")
      {
        fail("'$name' is defined both as a parameter and an attribute of an input stream") if defined $model->getParameterByName($name) ;

        return "({ int value = 0 ; { AutoMutex trackbar_lck(_trackbar_mutex) ; value = _auto_$atname ; } value ; })" ;
      }
    }
  }

  fail("'$name' is not a parameter nor an attribute of any input stream") unless defined $model->getParameterByName($name) ;

  my $val = get_param($model, $name, undef) ;

  fail("'$name' is undefined") unless defined $val ;
  
  return $val ;
}


sub ipl_schema($)
{
  my ($prefix) = @_ ;

  $prefix = $ipl_prefix unless $prefix ;

  return "${prefix}channels: int32, ${prefix}depth: int32, ${prefix}width: int32, ${prefix}height: int32, ${prefix}data: list<uint8>" ;
}

sub copy2tuple($$$)
{
  my ($prefix, $img, $tpl) = @_ ;

  $prefix = $ipl_prefix unless $prefix ;

  return "COPY2TUPLE_Q($img, $tpl, get_${prefix}channels, get_${prefix}depth, get_${prefix}width, get_${prefix}height, get_${prefix}data)" ;
}

sub copy2img($$$)
{
  my ($prefix, $tpl, $img) = @_ ;

  $prefix = $ipl_prefix unless $prefix ;

  return "COPY2IMG_Q($tpl, $img, get_${prefix}channels, get_${prefix}depth, get_${prefix}width, get_${prefix}height, get_${prefix}data)" ;
}

sub find_ipl_prefix($)
{
  my ($model) = @_ ;

  my %attr_names ;
  
  for(my $pid=0; $pid < $model->getNumberOfInputPorts( ); $pid++)
  {
    foreach my $attribute (@{$model->getInputPortAt($pid)->getAttributes( )})
    {
      $attr_names{$attribute->getName( )}++ ;
    }
  }

  for(my $pid=0; $pid < $model->getNumberOfOutputPorts( ); $pid++)
  {
    foreach my $attribute (@{$model->getOutputPortAt($pid)->getAttributes( )})
    {
      $attr_names{$attribute->getName( )}++ ;
    }
  }
  
  foreach my $k (keys %attr_names)
  {
    if ($k =~ m/^(.*)data$/)
    {
      my $prefix = $1 ;

      if ($attr_names{"${prefix}channels"} &&
          $attr_names{"${prefix}depth"} &&
          $attr_names{"${prefix}width"} &&
          $attr_names{"${prefix}height"})
      {
        print STDERR "IPL prefix found: $prefix\n" if $model->getContext()->isVerboseModeOn( ) ;

        $ipl_prefix = $prefix ;
        return $prefix ;
      }          
    }
  }

  die "Unable to find a schema resembling an IPL image" ;
}

1

# Local Variables: ***
# mode:perl ***
# perl-indent-level: 2 ***
# End: ***
