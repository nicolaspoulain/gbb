<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:template match="/">
    <html>
      <body>
        <xsl:apply-templates />
      </body>
    </html>
  </xsl:template>

<xsl:template match="title">
  <xsl:variable name="hierarchy" select="count(ancestor::categorie)"/>
  <xsl:variable name="heading">h<xsl:value-of select="$hierarchy" /></xsl:variable>

  <xsl:choose>
    <xsl:when test="$hierarchy > 6">
      <h6 class="{$heading}"><xsl:value-of select="." /></h6>
    </xsl:when>
    <xsl:otherwise>
      <xsl:element name="{$heading}">
        <xsl:value-of select="." />
      </xsl:element>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

  <xsl:template match="dcDispoId">-DCDISPOID : <xsl:value-of select="." />--</xsl:template>
  <xsl:template match="lib_dispo">-LIB DISPO : <xsl:value-of select="." />--</xsl:template>
  <xsl:template match="lobjt">-LIB OBJET : <xsl:value-of select="." />--</xsl:template>
  <xsl:template match="co_modu">-CO MODU   : <xsl:value-of select="." />--</xsl:template>
  <xsl:template match="lib_module">-LIB MODU  : <xsl:value-of select="." />--</xsl:template>
  <xsl:template match="duree_prev">-DUREE PREV: <xsl:value-of select="." />--</xsl:template>
  <xsl:template match="resppedagogique">-RESP PEDA : <xsl:value-of select="." />--</xsl:template>
  <xsl:template match="qualite">-QUALITE   : <xsl:value-of select="." />--</xsl:template>
  <xsl:template match="objectif">-OBJECTIF  : <xsl:value-of select="." />--</xsl:template>
  <xsl:template match="contenu">-CONTENU   : <xsl:value-of select="." />--</xsl:template>
  <xsl:template match="modalite">-MODALITE  : <xsl:value-of select="." />--</xsl:template>
  <xsl:template match="forme">-FORME     : <xsl:value-of select="." />--</xsl:template>

</xsl:stylesheet>

